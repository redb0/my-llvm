#include "Lexer.h"
#include <cstdlib>
#include <vector>
#include "tokens.h"


using token = TOKENS::token;

std::vector<token> slice(std::vector<token> v, int from, int to) {
    if (from == to) {
        return std::vector<token>{v.at(from)};
    }
    if (from < to) {
        return std::vector<token>{v.begin() + from, v.begin() + to + 1};
    }
}

//bool argumentsCheck(std::vector<token>& vec_tokens, int idx) {
//    token t = vec_tokens[idx];
//    if (holds_alternative<TOKENS::OPEN_PAREN>(t)) {
//        while (t != TOKENS::CLOSE_PAREN{}) {
//            idx++;
//            t = vec_tokens[idx];
//            if (t == TOKENS::IDENTIFIER{}) {}
//        }
//    }
//}

//bool isFuncDeclarationCorrect(std::vector<token>& vec_tokens) {
//    bool flag = true;
//    for (int i = 0; i < vec_tokens.size(); i++) {
//        if (vec_tokens[0] != TOKENS::DEF_FUN) {
//            return false;
//        }
//        if (vec_tokens[1] != TOKENS::IDENTIFIER) {
//            return false;
//        }
//        if (vec_tokens[2] != TOKENS::OPEN_PAREN) {
//            return false;
//        }


//    }
//}

std::vector<token> lexer(std::vector<token>& vec) { // изменить возвращаемый тип

}
