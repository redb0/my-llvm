#include "parser.h"


IntLiteralAST* parserIntNumberExpr(TOKENS::INT value,
                                   BaseAST* parent) {
    IntLiteralAST* res = new IntLiteralAST(value.var);
    res->setParent(parent);
    return res;
}

DoubleLiteralAST* parserDoubleNumberExpr(TOKENS::DOUBLE value,
                                         BaseAST* parent) {
    DoubleLiteralAST* res = new DoubleLiteralAST(value.var);
    res->setParent(parent);
    return res;
}

BoolLiteralAST* parserBoolNumberExpr(TOKENS::BOOL value, BaseAST* parent) {
    BoolLiteralAST* res = new BoolLiteralAST(value.var);
    res->setParent(parent);
    return res;
}


FunctionDeclarationAST* parserFuncExpr(std::vector<token>::iterator &current,
                                       std::vector<token>::iterator end,
                                       BaseAST* parent) {
    token lastToken = *current;
    std::vector<std::tuple<TypeAST*, std::string const *>> args;

    // std::tuple<int, double> myTuple;
    // std::get<2>(myTuple)

    TypeAST* t;
    if (holds_alternative<TOKENS::DEF_FUN>(lastToken)) {
        ++current;
        lastToken = *current;
        TOKENS::IDENTIFIER* funId = get_if<TOKENS::IDENTIFIER>(&lastToken);
        if (funId == nullptr) {
            return nullptr;
        }
        ++current;
        lastToken = *current;
        TOKENS::OPEN_PAREN* openParen = get_if<TOKENS::OPEN_PAREN>(&lastToken);
        if (openParen == nullptr) {
            return nullptr;
        } else {
            ++current;
            lastToken = *current;
            while (!holds_alternative<TOKENS::CLOSE_PAREN>(lastToken)) {
                if (!holds_alternative<TOKENS::COMMA>(lastToken)) {
                    for (int i = 0; i < 2; i++) {
                        std::tuple<TypeAST*, std::string> tuple;
                        TOKENS::IDENTIFIER* id = get_if<TOKENS::IDENTIFIER>(&lastToken);
                        if (id != nullptr) {
                            auto mt = parent->getNames(id->name);
                            TypeAST** type = get_if<TypeAST*>(&mt);
                            if (type == nullptr) {
                                std::get<0>(tuple) = *type;
                            } else {
                                std::get<1>(tuple) = id->name;
                            }
                        }
                        ++current;
                        lastToken = *current;
                    }
                } else {
                    ++current;
                    lastToken = *current;
                }
            }
            ++current;
            lastToken = *current;
            if (holds_alternative<TOKENS::ARROW>(lastToken)) {
                ++current;
                lastToken = *current;
                TOKENS::IDENTIFIER* type = get_if<TOKENS::IDENTIFIER>(&lastToken);
                if (type != nullptr) {
                    auto mt = parent->getNames(type->name);
                    auto type = get_if<TypeAST*>(&mt);
                    if (type != nullptr) {
                        t = *type;
                    }
                }
            }
            if (holds_alternative<TOKENS::OPEN_BRACES>(lastToken)) {
//                auto decl = new FunctionDeclarationAST(funId->name, args, t);
//                decl->setParent(parent);
//                return decl;
            }
            if (holds_alternative<TOKENS::SEMICOLON>(lastToken)) {
//                auto decl = new FunctionDeclarationAST(funId->name, args, t);
//                decl->setParent(parent);
//                return decl;
                // парсинг тела и ссылка на объявление функции
//                auto res = new FunctionDefinnitionAST(funId->name, args, t);
//                res->setParent(parent);
//                return res;
            }
        }
    }
}

template<class... Ts> struct overloaded : Ts...
{
    overloaded(Ts const& ... vals)
        : Ts (vals)...
    {

    }
//    using Ts::operator()...;
};
//template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
template <typename... Ts>
auto make_overloaded(Ts const& ... vals )
{
    return overloaded<Ts...>{vals...};
}

CompoundOperation* parserBodyFuncExpr(std::vector<token>::iterator &current,
                                     std::vector<token>::iterator end,
                                     BaseAST *parent) {
    token lastToken = *current;

    while (!holds_alternative<TOKENS::CLOSE_BRACES>(lastToken)) {

        if (holds_alternative<TOKENS::DEF_VAR>(lastToken)) {
            auto expr = parseVariableDecl(current, end, parent);
        }
        if (holds_alternative<TOKENS::IDENTIFIER>(lastToken)) {
            //вызов функции для разбора выражения или вызова функции
        }
        lastToken = *current;
        ++current;
    }
}

VariableDeclarationAST* parseVariableDecl(std::vector<token>::iterator &current,
                                          std::vector<token>::iterator end,
                                          BaseAST *parent) {
    token lastToken = *current;
    ++current;
    TOKENS::IDENTIFIER* id = get_if<TOKENS::IDENTIFIER>(&lastToken);
    if (id != nullptr) {
        auto it = parent->names.find(id->name);
        if (it != parent->names.end()) {
            return nullptr;
        }
        auto mt = parent->getNames(id->name);
        TypeAST** type = get_if<TypeAST*>(&mt);
        if ((type == nullptr) && (*type == nullptr)) {
            return nullptr;
        }
        lastToken = *current;
        mt = parent->getNames(id->name);
        type = get_if<TypeAST*>(&mt);
        if ((type == nullptr) && (*type == nullptr)) {
            return nullptr;
        }
        auto res = new VariableDeclarationAST(id->name, *type);
        res->setParent(parent);
        parent->addNames(id->name, res);
        return res;
    } else {
        return nullptr;
    }
}

UnaryAST* parseUnaryExpr(std::vector<token>::iterator &current,
                         std::vector<token>::iterator end,
                         BaseAST *parent) {
    token lastToken = *current;
    ++current;
    UnaryAST::UnOperator op;
    if (holds_alternative<TOKENS::OPEN_PAREN>(lastToken) ||
            holds_alternative<TOKENS::OPERATORS::AS>(lastToken)) {
        lastToken = *current;
        if  (holds_alternative<TOKENS::OPERATORS::ARITHMETICS::MN>(lastToken)) {
            op = UnaryAST::UnOperator::umn;
        }
        if  (holds_alternative<TOKENS::OPERATORS::LOGIC::NOT_>(lastToken)) {
            op = UnaryAST::UnOperator::not_;
        }
        ++current;
        lastToken = *current;
        TOKENS::IDENTIFIER* id = get_if<TOKENS::IDENTIFIER>(&lastToken);
        if (holds_alternative<TOKENS::IDENTIFIER>(lastToken)) {
            auto mv = parent->getNames(id->name);
            auto t = get_if<TypeAST*>(&mv);
            if (t != nullptr && *t != nullptr) {
//                UnaryAST* espr = new UnaryAST(op, mv);
//                espr->setParent(parent);
//                return espr;
            }
        }
        return nullptr;
    }
}

BinaryAST* parseBinaryOperation(token t, BaseAST *parent,
                                ExpressionAST* lhs) {
    BinaryAST* op = new BinaryAST(parent);
    // '+'
    if (holds_alternative<TOKENS::OPERATORS::ARITHMETICS::P>(t)) {
        op->setOperator(BinaryAST::BinOperator::add);
        op->setLHS(lhs);
        return op;
    }
    // '-'
    if (holds_alternative<TOKENS::OPERATORS::ARITHMETICS::MN>(t)) {
        op->setOperator(BinaryAST::BinOperator::sub);
        op->setLHS(lhs);
        return op;
    }
    // '/'
    if (holds_alternative<TOKENS::OPERATORS::ARITHMETICS::D>(t)) {
        op->setOperator(BinaryAST::BinOperator::div);
        op->setLHS(lhs);
        return op;
    }
    // '*'
    if (holds_alternative<TOKENS::OPERATORS::ARITHMETICS::MP>(t)) {
        op->setOperator(BinaryAST::BinOperator::mul);
        op->setLHS(lhs);
        return op;
    }
    // '&&'
    if (holds_alternative<TOKENS::OPERATORS::LOGIC::AND_>(t)) {
        op->setOperator(BinaryAST::BinOperator::and_);
        op->setLHS(lhs);
        return op;
    }
    // '||'
    if (holds_alternative<TOKENS::OPERATORS::LOGIC::OR_>(t)) {
        op->setOperator(BinaryAST::BinOperator::or_);
        op->setLHS(lhs);
        return op;
    }
    // '=='
    if (holds_alternative<TOKENS::OPERATORS::COMP::E>(t)) {
        op->setOperator(BinaryAST::BinOperator::e);
        op->setLHS(lhs);
        return op;
    }
    // '!='
    if (holds_alternative<TOKENS::OPERATORS::COMP::NE>(t)) {
        op->setOperator(BinaryAST::BinOperator::ne);
        op->setLHS(lhs);
        return op;
    }
    // '>'
    if (holds_alternative<TOKENS::OPERATORS::COMP::G>(t)) {
        op->setOperator(BinaryAST::BinOperator::g);
        op->setLHS(lhs);
        return op;
    }
    // '<'
    if (holds_alternative<TOKENS::OPERATORS::COMP::L>(t)) {
        op->setOperator(BinaryAST::BinOperator::l);
        op->setLHS(lhs);
        return op;
    }
    // '>='
    if (holds_alternative<TOKENS::OPERATORS::COMP::GE>(t)) {
        op->setOperator(BinaryAST::BinOperator::ge);
        op->setLHS(lhs);
        return op;
    }
    // '<='
    if (holds_alternative<TOKENS::OPERATORS::COMP::LE>(t)) {
        op->setOperator(BinaryAST::BinOperator::le);
        op->setLHS(lhs);
        return op;
    }
    return nullptr;
}

BinaryAST* parseInternalExpression(std::vector<token>::iterator &current,
                                   std::vector<token>::iterator end,
                                   BaseAST *parent) {
    token lastToken = *current;
    BinaryAST* rootOperation = nullptr;
    BinaryAST* lastOperation = nullptr;
    BinaryAST* op = nullptr;
    while (!holds_alternative<TOKENS::CLOSE_PAREN>(lastToken)) {
        if (holds_alternative<TOKENS::INT>(lastToken)) {
            TOKENS::INT* v = get_if<TOKENS::INT>(&lastToken);
            IntLiteralAST* operand = new IntLiteralAST(v->var); // ошибка
            ++current;
            lastToken = *current;
            if (holds_alternative<TOKENS::CLOSE_PAREN>(lastToken) &&
                    (lastOperation != nullptr)) {
                lastOperation->setRHS(operand);
                break;
            } else {
                op = parseBinaryOperation(*current, parent, operand);
            }
        }
        if (holds_alternative<TOKENS::DOUBLE>(lastToken)) {
            TOKENS::DOUBLE* v = get_if<TOKENS::DOUBLE>(&lastToken);
            DoubleLiteralAST* operand = new DoubleLiteralAST(v->var); // ошибка
            ++current;
            lastToken = *current;
            if (holds_alternative<TOKENS::CLOSE_PAREN>(lastToken) &&
                    (lastOperation != nullptr)) {
                lastOperation->setRHS(operand);
                break;
            } else {
                op = parseBinaryOperation(*current, parent, operand);
            }
        }
        if (holds_alternative<TOKENS::IDENTIFIER>(lastToken)) {
            TOKENS::IDENTIFIER* id = get_if<TOKENS::IDENTIFIER>(&lastToken);
            auto mv = parent->getNames(id->name);
            VariableDeclarationAST** operandDec = get_if<VariableDeclarationAST*>(&mv);
            VariableDefinitionAST* operand = new VariableDefinitionAST(*operandDec);
            if (operand == nullptr) {
                return nullptr;
            }
            ++current;
            lastToken = *current;
            if (holds_alternative<TOKENS::CLOSE_PAREN>(lastToken) &&
                    (lastOperation != nullptr)) {
                lastOperation->setRHS(operand);
                break;
            } else {
                op = parseBinaryOperation(*current, parent, operand);
            }
        }
        if (lastOperation == nullptr) {
            lastOperation = op;
        } else {
            lastOperation->setRHS(op);
            lastOperation = op;
        }
        if (rootOperation == nullptr) {
            rootOperation = op;
        }
        ++current;
        lastToken = *current;
    }
    return rootOperation;
}



CompoundOperation* parserExpression(std::vector<token>::iterator &current,
                                    std::vector<token>::iterator end,
                                    BaseAST *parent) {
    token lastToken = *current;
    BinaryAST* rootOperation = nullptr;
    BinaryAST* lastOperation = nullptr;

    while (!holds_alternative<TOKENS::SEMICOLON>(lastToken)) {
        if (holds_alternative<TOKENS::IDENTIFIER>(lastToken)) {
            ++current;
            //проверим, объявлено ли имя переменной, если нет - ошибка
            TOKENS::IDENTIFIER* id = get_if<TOKENS::IDENTIFIER>(&lastToken);
            auto mv = parent->getNames(id->name);
            VariableDeclarationAST** varDec = get_if<VariableDeclarationAST*>(&mv);
            if ((varDec == nullptr) && (*varDec == nullptr)) {
                return nullptr;
            }
            VariableDefinitionAST* varDef = new VariableDefinitionAST(*varDec);
            //если после идентификатора встретися оператор присваивания
            if (holds_alternative<TOKENS::OPERATORS::AS>(*current)) {
                BinaryAST* asOp = new BinaryAST(BinaryAST::BinOperator::as, parent);
                asOp->setLHS(varDef);
                if (lastOperation == nullptr) {
                    lastOperation = asOp;
                } else {
                    return nullptr;
                }
                if (rootOperation == nullptr) {
                    rootOperation = asOp;
                }
            }
            //если после идентификатора встретися оператор
            //'+', '-', '*', '/', '', '', ''
            BinaryAST* op = parseBinaryOperation(*current, parent, varDef);
            if (lastOperation == nullptr) {
                lastOperation = op;
            } else {
                lastOperation->setRHS(op);
            }
            if (rootOperation == nullptr) {
                rootOperation = op;
            }

            if (holds_alternative<TOKENS::OPEN_PAREN>(*current)) {
                //создать вызов функции
            }
        }
        if (holds_alternative<TOKENS::OPEN_PAREN>(lastToken)
                && (lastOperation != nullptr)) {
            //если встретилась открывающая скобка и перед ней был знак оператора,
            //значит это составная операция,
            //вызвать функцию разбора операции в скобках
            BinaryAST* op = parseInternalExpression(current, end, lastOperation);
        }
        ++current;
        lastToken = *current;
    }
}
