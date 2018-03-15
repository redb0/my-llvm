#ifndef _AST_H_
#define _AST_H_
#include <string>
#include <memory>
#include <vector>
#include <map>
#include "mpark/variant.hpp"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include <llvm/IR/Constants.h>
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

using llvm::Value;
using llvm::Function;
using mpark::variant;

class TypeAST;
class VariableDeclarationAST;
class VariableDefinitionAST;

class BaseAST {
public:
    BaseAST* parent = nullptr;
    std::map<std::string,
             variant<TypeAST*,
             VariableDeclarationAST*,
             VariableDefinitionAST*>> names;
    BaseAST() = default;
    BaseAST(BaseAST const&) = default;
    virtual ~BaseAST() {}
    virtual Value* codegen() = 0;
    virtual bool valid() {return true;}
    variant<TypeAST*, VariableDeclarationAST*,
            VariableDefinitionAST*> getNames(std::string n) {
        auto it = names.find(n);
        if ((it == names.end()) && (parent != nullptr)) {
            return parent->getNames(n);
        }
        if (it != names.end()) {
            return it->second;
        }
        if ((it == names.end()) && (parent == nullptr)) {
            return static_cast<TypeAST*>(nullptr);
        }
    }
    void addNames(std::string name, variant<TypeAST*,
                  VariableDeclarationAST*,
                  VariableDefinitionAST*> ptr) {
        this->names[name] = ptr;
    }
    void setParent(BaseAST* parent) {
        this->parent = parent;
    }
};

class TypeAST : public BaseAST {
public:
    std::string name;
    TypeAST(std::string const& name)
        : name(name)
    {}
    virtual Value* codegen() = 0;
};

class IntType : public TypeAST {
public:
    IntType()
        : TypeAST("int")
    {}
    virtual Value* codegen() override;
    static IntType* getInstance();
};

class DoubleType : public TypeAST {
public:
    DoubleType()
        : TypeAST("double")
    {}
    virtual Value* codegen() override;
    static DoubleType* getInstance();
};

class BoolType : public TypeAST {
public:
    BoolType()
        : TypeAST("bool")
    {}
    virtual Value* codegen() override;
    static BoolType* getInstance();
};

class ExpressionAST: public BaseAST {
public:
    virtual TypeAST* getType() = 0;
    virtual Value* codegen() = 0;
};

class IntLiteralAST : public ExpressionAST {
public:
    int value;
//    BaseAST* parent = nullptr;
    IntLiteralAST(int v)
        : value(v)
    {}
    Value *codegen();
    virtual TypeAST* getType();
};

class BoolLiteralAST: public ExpressionAST {
public:
    bool value;
//    BaseAST* parent = nullptr;
    BoolLiteralAST(bool v)
        : value(v)
    {}
    Value *codegen() override;
    virtual TypeAST* getType();
};

class DoubleLiteralAST : public ExpressionAST {
public:
    double value;
//    BaseAST* parent = nullptr;
    DoubleLiteralAST(double v)
        : value(v)
    {}
    Value* codegen() override;
    virtual TypeAST* getType();
};

class VariableDeclarationAST : public BaseAST { //объявление
public:
    std::string name;
    TypeAST* type;
    VariableDeclarationAST(std::string const& name, TypeAST* type)
        : name(name), type(type)
    {}
    virtual Value* codegen() override;
    virtual bool valid() override;
};

class VariableDefinitionAST : public ExpressionAST { //
public:
    VariableDeclarationAST* variable;
    VariableDefinitionAST(VariableDeclarationAST* v)
        : variable(v)
    {}
    virtual Value* codegen() override;
    virtual TypeAST* getType() override;
    virtual bool valid() override;
};

class CompoundOperation: public BaseAST {
public:
    std::vector<BaseAST*> operations;
    CompoundOperation() = default;
    CompoundOperation(std::vector<BaseAST*> ops)
        : operations(ops)
    {}
    virtual Value* codegen() override;
    virtual bool valid() override;
};

class BinaryAST: public ExpressionAST {
public:
    enum class BinOperator{add, sub, div, mul,
                           and_, or_,
                           e, ne, g, l, ge, le,
                           as};
    BinOperator binOp;
    ExpressionAST* lhs;
    ExpressionAST* rhs;
    BaseAST* parent;
    BinaryAST(BaseAST* parent)
        :parent(parent)
    {}
    BinaryAST(BinOperator op, BaseAST* parent)
        : binOp(op), parent(parent)
    {}
    BinaryAST(BinOperator op, ExpressionAST* lhs,
              ExpressionAST* rhs, BaseAST* parent)
        : binOp(op), lhs(lhs), rhs(rhs), parent(parent)
    {}
    void setLHS(ExpressionAST* lhs) {
        this->lhs = lhs;
    }
    void setRHS(ExpressionAST* rhs) {
        this->rhs = rhs;
    }
    void setOperator(BinOperator op) {
        this->binOp = op;
    }
    virtual Value* codegen() override;
    virtual TypeAST* getType() override;
    virtual bool valid() override;
};

class UnaryAST: public ExpressionAST {
public:
    enum class UnOperator{up, umn, not_};
    UnOperator unOp;
    ExpressionAST* rhs;
    UnaryAST(UnOperator op, ExpressionAST* rhs)
        : unOp(op), rhs(rhs)
    {}
    virtual Value* codegen() override;
    virtual TypeAST* getType() override;
    virtual bool valid() override;
};

class FunctionDeclarationAST: public BaseAST { // объявдение функции
public:
    std::string name;
    std::vector<TypeAST*> args; //
    TypeAST* return_type;
    FunctionDeclarationAST(std::string name,
                           const std::vector<TypeAST*> args,
                           TypeAST* t)
        : name(name), args(args), return_type(t)
    {}
    Function* codegen();
};

class FunctionDefinnitionAST: public BaseAST { // определение функции
public:
    FunctionDeclarationAST* funcDeclaration;
    std::vector<BaseAST*> args;
    CompoundOperation* body;
    FunctionDefinnitionAST(FunctionDeclarationAST* proto,
                           std::vector<BaseAST*> a,
                           CompoundOperation* b)
        : funcDeclaration(proto), args(a), body(b)
    {}
    Function* codegen();
};

class FunctionCallAST: public ExpressionAST {
public:
    FunctionDeclarationAST* funcDeclaration;
    std::vector<BaseAST*> args;
    FunctionCallAST(FunctionDeclarationAST* func, std::vector<BaseAST*> a)
        : funcDeclaration(func), args(a)
    {}
    virtual Value* codegen() override;
    virtual TypeAST* getType() override;
    virtual bool valid() override;
};

class IfAST: public BaseAST {
public:
    ExpressionAST* expression;
    CompoundOperation* body;
    CompoundOperation* elseBody;
    IfAST(ExpressionAST* e, CompoundOperation* b, CompoundOperation* eb)
        : expression(e), body(b), elseBody(eb)
    {}
    virtual Value* codegen() override;
    virtual bool valid() override;
};

class WhileAST: public BaseAST {
public:
    ExpressionAST* expression;
    CompoundOperation* body;
    WhileAST(ExpressionAST* e, CompoundOperation* body)
        : expression(e), body(body)
    {}
    virtual Value* codegen() override;
    virtual bool valid() override;
};

class DoAST: public BaseAST {
public:
    ExpressionAST* expression;
    CompoundOperation* body;
    DoAST(ExpressionAST* e, CompoundOperation* b)
        : expression(e), body(b)
    {}
    virtual Value* codegen() override;
    virtual bool valid() override;
};


#endif //_AST_H_
