#ifndef NODE_HPP
#define NODE_HPP

#include <llvm/IR/Value.h>

#include <iostream>
#include <vector>

class CodeGenContext;

class Node {
   public:
    virtual ~Node() {}
    virtual llvm::Value* codeGen(CodeGenContext& context) {}
    friend std::ostream& operator<<(std::ostream& os, const Node& node) {
        return node.print(os);
    }

   private:
    virtual std::ostream& print(std::ostream& os) const {
        os << "Node()";
        return os;
    }
};

class Declaration : public Node {};
class Expression : public Node {};
class Statement : public Declaration {};

typedef std::vector<Declaration*> DeclarationList;
typedef std::vector<Statement*> StatementList;
typedef std::vector<Expression> ExpressionList;

class Program : public Node {
   public:
    DeclarationList decls;
    Program() {}
    virtual llvm::Value* codeGen(CodeGenContext& context);

   private:
    virtual std::ostream& print(std::ostream& os) const {
        os << "Program";
        return os;
    }
};

class Number : public Expression {
   public:
    double value;
    Number(double value) : value(value) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);

   private:
    virtual std::ostream& print(std::ostream& os) const {
        os << "Number(" << value << ")";
        return os;
    }
};

class Binary : public Expression {
   public:
    int op;
    Expression& lhs;
    Expression& rhs;
    Binary(Expression& lhs, int op, Expression& rhs)
        : lhs(lhs), rhs(rhs), op(op) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);

   private:
    virtual std::ostream& print(std::ostream& os) const {
        os << "Binary(" << lhs << " " << op << " " << rhs << ")";
        return os;
    }
};

class Print : public Statement {
   public:
    Expression& expr;
    Print(Expression& expr) : expr(expr) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);

   private:
    virtual std::ostream& print(std::ostream& os) const {
        os << "PrintStmt";
        return os;
    }
};

#endif