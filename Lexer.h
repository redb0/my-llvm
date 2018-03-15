#ifndef _LEXER_H_
#define _LEXER_H_

#include <cstdlib>
#include <vector>
#include "tokens.h"


using token = TOKENS::token;

std::vector<token> doLexing(std::vector<token>);

std::vector<token> slice(std::vector<token>, int, int);


#endif //_LEXER_H_
