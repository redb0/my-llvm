#include "ast.h"

#include <algorithm>
#include <initializer_list>

#include "tokenizer.h"
#include "parser.h"

namespace{
    template<typename Tc, typename To>
    inline
    bool isIn2(To const& obj, Tc const& cntr) {
        return std::find(std::begin(cntr), std::end(cntr), obj);
    }
    template<typename To>
    inline
    bool isIn2(To const& obj, std::initializer_list<To> const& cntr) {
        return std::find(std::begin(cntr), std::end(cntr), obj);
    }
}


TypeAST* UnaryAST::getType() {
    return rhs->getType();
}

TypeAST* BinaryAST::getType() {
    if ((binOp == BinOperator::g) || (binOp == BinOperator::ge) ||
            (binOp == BinOperator::l) || (binOp == BinOperator::le)) {
        return BoolType::getInstance();
    } else {
        return lhs->getType();
    }
}

TypeAST* VariableDefinitionAST::getType() {
    return variable->type;
}

TypeAST* FunctionCallAST::getType() {
    return funcDeclaration->return_type;
}

bool BinaryAST::valid() {
    TypeAST* lhsType = lhs->getType();
    TypeAST* rhsType = lhs->getType();
    if (isIn2(binOp, {BinOperator::add, BinOperator::sub,
                      BinOperator::mul, BinOperator::div}) ){
        // dynamic_cast<IntType*>(lhsType) != nullptr
        if (dynamic_cast<IntType*>(lhsType)
                && dynamic_cast<IntType*>(rhsType)) {
            return true;
        }
        if (dynamic_cast<DoubleType*>(lhsType)
                && dynamic_cast<DoubleType*>(rhsType)) {
            return true;
        }
        return false;
    }
    if ((binOp == BinOperator::and_) || (binOp == BinOperator::or_) ||
            (binOp == BinOperator::e) || (binOp == BinOperator::ne)) {
        if (dynamic_cast<BoolType*>(lhsType) && dynamic_cast<BoolType*>(rhsType)) {
            return true;
        }
        return false;
    }
    if ((binOp == BinOperator::g) || (binOp == BinOperator::ge) ||
            (binOp == BinOperator::l) || (binOp == BinOperator::le)) {
        if ((dynamic_cast<IntType*>(lhsType) || dynamic_cast<DoubleType*>(lhsType)) &&
                (dynamic_cast<IntType*>(rhsType) || dynamic_cast<DoubleType*>(rhsType))) {
            return true;
        }
        return false;
    }
}

namespace {
    IntType IntInstance;
    BoolType BoolInstance;
    DoubleType DoubleInstance;
}

Value *IntType::codegen() {}

IntType *IntType::getInstance() {
    return &IntInstance;
}

Value *DoubleType::codegen() {}

DoubleType *DoubleType::getInstance() {
    return &DoubleInstance;
}

Value *BoolType::codegen() {}

BoolType *BoolType::getInstance() {
    return &BoolInstance;
}

static llvm::LLVMContext TheContext;
static llvm::IRBuilder<> Builder(TheContext);
static std::unique_ptr<llvm::Module> TheModule;
static std::map<std::string, Value *> NamedValues;

//константы числовые
Value *IntLiteralAST::codegen() {
    return llvm::ConstantInt::get(TheContext, llvm::APInt(64, value));
}
Value *DoubleLiteralAST::codegen() {
    return llvm::ConstantFP::get(TheContext, llvm::APFloat(value));
}
//Value *BoolLiteralAST::codegen() { //нет bool
//    return llvm::ConstantFP::get(TheContext, llvm::APFloat(value));
//}

Value *BinaryAST::codegen() {
    Value *L = lhs->codegen();
    Value *R = rhs->codegen();
    if (!L || !R)
        return nullptr;
    switch (binOp) {
    //арифметические операции
    case BinaryAST::BinOperator::add: // '+'
        return Builder.CreateFAdd(L, R, "addtmp");
    case BinaryAST::BinOperator::sub: // '-'
        return Builder.CreateFSub(L, R, "subtmp");
    case BinaryAST::BinOperator::mul: // '*'
        return Builder.CreateFMul(L, R, "multmp");
    case BinaryAST::BinOperator::div: // '/'
        return Builder.CreateFDiv(L, R, "divtmp");
    //логические операции
    case BinaryAST::BinOperator::and_: // 'and'
        return Builder.CreateAnd(L, R, "andtmp");
    case BinaryAST::BinOperator::or_: // 'or'
        return Builder.CreateOr(L, R, "ortmp");
    //операции сравнения. никаких неявных преобразование
    case BinaryAST::BinOperator::l: // '<'
        return Builder.CreateFCmpULT(L, R, "cmpULTtmp");
        // Convert bool 0/1 to double 0.0 or 1.0
        // return Builder.CreateUIToFP(L, llvm::Type::getDoubleTy(TheContext), "booltmp");
    case BinaryAST::BinOperator::g: // '>'
        return Builder.CreateFCmpUGT(L, R, "cmpUGTtmp");
        // return Builder.CreateUIToFP(L, llvm::Type::getDoubleTy(TheContext), "booltmp");
    case BinaryAST::BinOperator::e: // '=='
        return Builder.CreateFCmpOEQ(L, R, "cmpOEQtmp");
        // return Builder.CreateUIToFP(L, llvm::Type::getDoubleTy(TheContext), "booltmp");
    case BinaryAST::BinOperator::ne: // '!=='
        return Builder.CreateFCmpONE(L, R, "cmpONEtmp");
        // return Builder.CreateUIToFP(L, llvm::Type::getDoubleTy(TheContext), "booltmp");
    case BinaryAST::BinOperator::ge: // '>='
        return Builder.CreateFCmpUGE(L, R, "cmpUGEtmp");
        // return Builder.CreateUIToFP(L, llvm::Type::getDoubleTy(TheContext), "booltmp");
    case BinaryAST::BinOperator::le: // '<='
        return Builder.CreateFCmpULE(L, R, "cmpULEtmp");
        // return Builder.CreateUIToFP(L, llvm::Type::getDoubleTy(TheContext), "booltmp");
    //  default:
    //    return LogErrorV("invalid binary operator");
    }
}

// top ::= definition | external | expression | ';' | END_OF_FILE
static void MainLoop(std::vector<token> vec) {
    std::vector<token>::iterator current = vec.begin();
    std::vector<token>::iterator end = vec.end();
    CompoundOperation* root_obj = new CompoundOperation{}; // корневой элемент
    token lastToken;

    while (current != end) {
        fprintf(stderr, "ready> ");
        lastToken = *current;
        if (holds_alternative<TOKENS::END_OF_FILE>(lastToken)) {
            return; // конец файла
        }
        if (holds_alternative<TOKENS::SEMICOLON>(lastToken)) {
            ++current; //пропуск ;
            lastToken = *current;
        }

        if (holds_alternative<TOKENS::DEF_FUN>(lastToken)) {
            parserFuncExpr(current, end, root_obj);
            ++current; //
            lastToken = *current;
        }
        if (holds_alternative<TOKENS::DEF_VAR>(lastToken)) {
            parseVariableDecl(current, end, root_obj);
            ++current; //
            lastToken = *current;
        }

        if (holds_alternative<TOKENS::IDENTIFIER>(lastToken)) {

            ++current; //здесь парсинг выражения
            lastToken = *current;
        }
        if (holds_alternative<TOKENS::OPEN_PAREN>(lastToken) ||
                holds_alternative<TOKENS::OPEN_BRACES>(lastToken)) {
            CompoundOperation* op = parserExpression(current, end, root_obj);

            //парсинг составного оператора
            ++current;
            lastToken = *current;
        }
    }
}

int main() {
  // получение массива с токенами.
  fprintf(stderr, "ready> ");

  std::string src;
  std::string file_name = "code.txt";
  src = open_file(file_name);
  std::vector<token> vec{};
  vec = get_tokens(src);

  // модуль который содержит весь код.
  TheModule = llvm::make_unique<llvm::Module>("my cool jit", TheContext);

  // Run the main "interpreter loop" now.
  MainLoop(vec);

  // Вывод сгенерированного кода.
  TheModule->print(llvm::errs(), nullptr);

  return 0;
}
