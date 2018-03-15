#ifndef _PARSER_H_
#define _PARSER_H_

#include <memory>
#include "ast.h"
#include "tokens.h"
//#include "tokenizer.h"

using token = TOKENS::token;

IntLiteralAST *parserIntNumberExpr(TOKENS::INT value,
                                   BaseAST* parent); //+

DoubleLiteralAST* parserDoubleNumberExpr(TOKENS::DOUBLE value,
                                         BaseAST* parent); //+

BoolLiteralAST* parserBoolNumberExpr(TOKENS::BOOL value,
                                     BaseAST *parent); //+


VariableDeclarationAST* parseVariableDecl(std::vector<token>::iterator& current,
                                          std::vector<token>::iterator end,
                                          BaseAST* parent);


FunctionDeclarationAST* parserFuncExpr(std::vector<token>::iterator& current,
                                       std::vector<token>::iterator end,
                                       BaseAST* parent); //+

CompoundOperation* parserBodyFuncExpr(std::vector<token>::iterator& current,
                                      std::vector<token>::iterator end,
                                      BaseAST* parent);

CompoundOperation* parserExpression(std::vector<token>::iterator& current,
                                    std::vector<token>::iterator end,
                                    BaseAST* parent);

#endif //_PARSER_H_
