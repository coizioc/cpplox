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

class Statement : public Node {};
class Expr : public Node {};

typedef std::vector<Statement*> StatementList;
typedef std::vector<Expr> ExprList;

class Program : public Node {
   public:
    StatementList decls;
    Program() {}
    virtual llvm::Value* codeGen(CodeGenContext& context);

   private:
    virtual std::ostream& print(std::ostream& os) const {
        os << "Program()";
        return os;
    }
};

class Number : public Expr {
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

class Variable : public Expr {
   public:
    std::string name;
    Variable(const std::string& name) : name(name) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);

   private:
    virtual std::ostream& print(std::ostream& os) const {
        os << "Variable(" << name << ")";
        return os;
    }
};

class Assign : public Expr {
   public:
    std::string name;
    Expr& rhs;
    Assign(std::string name, Expr& rhs) : name(name), rhs(rhs) {}
    Assign(Variable& variable, Expr& rhs) : name(variable.name), rhs(rhs) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);

   private:
    virtual std::ostream& print(std::ostream& os) const {
        os << "Assign(" << name << " = " << rhs << ")";
        return os;
    }
};

class Binary : public Expr {
   public:
    int op;
    Expr& lhs;
    Expr& rhs;
    Binary(Expr& lhs, int op, Expr& rhs) : lhs(lhs), rhs(rhs), op(op) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);

   private:
    virtual std::ostream& print(std::ostream& os) const {
        os << "Binary(" << lhs << " " << op << " " << rhs << ")";
        return os;
    }
};

class Var : public Statement {
   public:
    std::string name;
    Expr& initializer;
    Var(Variable& variable, Expr& initializer)
        : name(variable.name), initializer(initializer) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);

   private:
    virtual std::ostream& print(std::ostream& os) const {
        os << "VarDecl(" << name << " = " << initializer << ")";
        return os;
    }
};

class Expression : public Statement {
   public:
    Expr& expr;
    Expression(Expr& expr) : expr(expr) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);

   private:
    virtual std::ostream& print(std::ostream& os) const {
        os << "ExprStmt()";
        return os;
    }
};

class Print : public Statement {
   public:
    Expr& expr;
    Print(Expr& expr) : expr(expr) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);

   private:
    virtual std::ostream& print(std::ostream& os) const {
        os << "PrintStmt()";
        return os;
    }
};

#endif