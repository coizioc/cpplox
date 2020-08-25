#include "codegen.hpp"

#include "node.hpp"
#include "parser.hpp"

std::unique_ptr<Module> CodeGenContext::TheModule =
    std::make_unique<Module>("main", TheContext);

void CodeGenContext::generateCode(Program& root) {
#ifdef DEBUG
    std::cout << "Generating code..." << std::endl;
#endif

    /* Create the top level interpreter function to call as entry */
    std::vector<Type*> argTypes;
    FunctionType* ftype =
        FunctionType::get(Type::getVoidTy(TheContext), argTypes, false);
    mainFunction = Function::Create(ftype, GlobalValue::ExternalLinkage,
                                    Twine("main"), *TheModule);
    BasicBlock* bblock =
        BasicBlock::Create(TheContext, "entry", mainFunction, 0);
    Builder.SetInsertPoint(bblock);

    /* Push a new variable/block context */
    pushBlock(bblock);
    root.codeGen(*this); /* emit bytecode for the toplevel block */
    ReturnInst::Create(TheContext, bblock);
    popBlock();

    /* Print the bytecode in a human-readable format
       to see if our program compiled properly
     */
#ifdef DEBUG
    std::cout << "Code is generated." << std::endl;
#endif
    legacy::PassManager pm;
    pm.add(createPrintModulePass(outs()));
    pm.run(*TheModule);
}

/* Executes the AST by running the main function */
GenericValue CodeGenContext::runCode() {
    std::cout << "Running code..." << std::endl;

    auto eb = std::make_unique<EngineBuilder>();
    eb->setEngineKind(EngineKind::JIT);
    ExecutionEngine* ee = eb->create();
    assert(ee != nullptr);
    std::cout << "Made execution engine..." << std::endl;

    std::vector<GenericValue> noargs;
    GenericValue v = ee->runFunction(mainFunction, noargs);
    std::cout << "Code was run." << std::endl;
    return v;
}

Value* Binary::codeGen(CodeGenContext& context) {
#ifdef DEBUG
    std::cout << "Creating binary " << *this << std::endl;
#endif
    switch (op) {
        case PLUS:
            return Builder.CreateFAdd(lhs.codeGen(context),
                                      rhs.codeGen(context), "");
        case MINUS:
            return Builder.CreateFSub(lhs.codeGen(context),
                                      rhs.codeGen(context), "");
        case STAR:
            return Builder.CreateFMul(lhs.codeGen(context),
                                      rhs.codeGen(context), "");
        case SLASH:
            return Builder.CreateFDiv(lhs.codeGen(context),
                                      rhs.codeGen(context), "");
        default:
            fprintf(stderr, "Unsupported operation: '%c'.\n", op);
            exit(1);
    }
}

Value* Number::codeGen(CodeGenContext& context) {
#ifdef DEBUG
    std::cout << "Creating number" << value << std::endl;
#endif
    return ConstantFP::get(TheContext, APFloat(value));
}

Value* Print::codeGen(CodeGenContext& context) {
#ifdef DEBUG
    std::cout << "Creating print" << std::endl;
#endif
    FunctionCallee printF = context.TheModule->getOrInsertFunction(
        "printf", FunctionType::get(
                      IntegerType::getInt32Ty(TheContext),
                      PointerType::get(Type::getInt8Ty(TheContext), 0), true));
    std::vector<Value*> ArgsV;

    ArgsV.push_back(Builder.CreateGlobalStringPtr("%g\n", "str"));
    ArgsV.push_back(expr.codeGen(context));

    return Builder.CreateCall(printF, ArgsV, "printfCall");
}

Value* Program::codeGen(CodeGenContext& context) {
    DeclarationList::const_iterator it;
    Value* last = nullptr;
    for (it = decls.begin(); it != decls.end(); it++) {
        // std::cout << "Generating code for " << typeid(**it).name() <<
        // std::endl;
        last = (**it).codeGen(context);
    }
#ifdef DEBUG
    std::cout << "Creating block" << std::endl;
#endif
    return last;
}
