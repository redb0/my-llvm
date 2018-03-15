#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <vector>

#include "tokens.h"

using token = TOKENS::token;

bool isIn(char c, std::string const& str);

token getTokenArithmeticsOp(char c);

token getTokenBrackets(char c);

token get_token(std::string::iterator& current,
                std::string::iterator end);

std::string open_file(std::string file_name);

std::vector<token> get_tokens(std::string s);

#endif //_TOKENIZER_H_
