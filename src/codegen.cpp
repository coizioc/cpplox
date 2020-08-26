#include "codegen.hpp"

#include "node.hpp"
#include "parser.hpp"

#define PRINT_CREATING_THIS() \
    std::cout << "Creating " << *this << "..." << std::endl

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

Value* Assign::codeGen(CodeGenContext& context) {
#ifdef DEBUG
    PRINT_CREATING_THIS();
#endif
    if (context.locals().find(variable.name) == context.locals().end()) {
        std::cerr << "undeclared variable " << variable.name << std::endl;
        return NULL;
    }
    auto store = Builder.CreateStore(rhs.codeGen(context), context.locals()[variable.name]);
    return store;
}

Value* Binary::codeGen(CodeGenContext& context) {
#ifdef DEBUG
    PRINT_CREATING_THIS();
#endif
    switch (op) {
        case EQUAL: {
            Variable& lhsCast = static_cast<Variable&>(lhs);
            Value* val = rhs.codeGen(context);
            if(val == nullptr) {
                return nullptr;
            }

            if(context.locals().find(lhsCast.name) == context.locals().end()) {
                std::cerr << "Variable " << lhsCast.name << " not found." << std::endl;
                exit(1);
            }
            Value* variable = context.locals()[lhsCast.name];
            Builder.CreateStore(val, variable);
            return val;
        }
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
    PRINT_CREATING_THIS();
#endif
    return ConstantFP::get(TheContext, APFloat(value));
}

Value* Variable::codeGen(CodeGenContext& context) {
#ifdef DEBUG
    PRINT_CREATING_THIS();
#endif
    if (context.locals().find(name) == context.locals().end()) {
        std::cerr << "Undeclared variable: " << name << std::endl;
        return NULL;
    }
    return Builder.CreateLoad(context.locals()[name], "");
}

Value* Expression::codeGen(CodeGenContext& context) {
#ifdef DEBUG
    PRINT_CREATING_THIS();
#endif
    return expr.codeGen(context);
}

Value* Print::codeGen(CodeGenContext& context) {
#ifdef DEBUG
    PRINT_CREATING_THIS();
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

Value* Var::codeGen(CodeGenContext& context) {
#ifdef DEBUG
    PRINT_CREATING_THIS();
#endif
    Value* initializedValue = initializer.codeGen(context);

    auto alloc = Builder.CreateAlloca(initializedValue->getType(), 0, variable.name);
    context.locals()[variable.name] = alloc;
    auto assign = Binary(variable, EQUAL, initializer);
    auto res = assign.codeGen(context);

    return res;
}

Value* Program::codeGen(CodeGenContext& context) {
#ifdef DEBUG
    PRINT_CREATING_THIS();
#endif
    StatementList::const_iterator it;
    Value* last = nullptr;
    for (it = decls.begin(); it != decls.end(); it++) {
        last = (**it).codeGen(context);
    }
    return last;
}
