#include "tokenizer.h"

#include <iostream>
#include <cctype>
#include <map>
#include <string>
#include <deque>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include <cstdlib>


#include "mpark/variant.hpp"
#include "Lexer.h"


using namespace std;
using std::string;

bool isIn(char c, string const& str) {
    return std::find(str.begin(), str.end(), c) != str.end();
}

token getTokenArithmeticsOp(char c) {
    if ('+' == c) {
        return TOKENS::OPERATORS::ARITHMETICS::P{};
    }
    if ('*' == c) {
        return TOKENS::OPERATORS::ARITHMETICS::MP{};
    }
    if ('/' == c) {
        return TOKENS::OPERATORS::ARITHMETICS::D{};
    }
}

token getTokenBrackets(char c) {
    if ('(' == c) {
        return TOKENS::OPEN_PAREN{};
    }
    if (')' == c) {
        return TOKENS::CLOSE_PAREN{};
    }
    if ('{' == c) {
        return TOKENS::OPEN_BRACES{};
    }
    if ('}' == c) {
        return TOKENS::CLOSE_BRACES{};
    }
}

token get_token(std::string::iterator& current,
                std::string::iterator end) {
    string IdentifierString = "";
    string NumStr = "";
    string OpSTR = "";
    char LastChar = ' ';

    // isspace() - считает '\n' пробелом, из-за чего они удаляются
    current = std::find_if(current, end, [](char c) {
        return !isspace(c);
    });
    if (current == end) {
        return TOKENS::END_OF_FILE{};
    }
    LastChar = *current;
    ++current;

    // комментарии сразу пропускаются
    if ('#' == LastChar) {
        do {
            LastChar = *current;
            ++current;
        } while ((current != end) || LastChar != '\n');
    }
    if (isIn(LastChar, "(){}")) {
        token t = getTokenBrackets(LastChar);
        cout << "Brackets - (){}" << endl;
        return t;
    }
    if (';' == LastChar) {
        cout << "SEMICOLON - ;" << endl;
        return TOKENS::SEMICOLON{};
    }
    if ('\n' == LastChar) {
        cout << "END_OF_LINE" << endl;
        return TOKENS::END_OF_LINE{};
    }
    if (',' == LastChar) {
        cout << "COMMA - ," << endl;
        return TOKENS::COMMA{};
    }
    if (isIn(LastChar, "+*/")) {
        token t = getTokenArithmeticsOp(LastChar);
        cout << "ArithmeticsOp - +-*/" << endl;
        return t;
    }
    if ((isIn(LastChar, "!<>=|&-")) && (OpSTR == "")) {
        cout << "OPERATORS - ||, &&, =, ==, !, !=, >=, <= " << endl;
        OpSTR = LastChar;
        ++current;
        LastChar = *current;
        while (isIn(LastChar, "!<>=|&")) {
            OpSTR += LastChar;
            ++current;
            LastChar = *current;
        }
        if (OpSTR == "||") {
            return TOKENS::OPERATORS::LOGIC::OR_{};
        }
        if (OpSTR == "&&") {
            return TOKENS::OPERATORS::LOGIC::AND_{};
        }
        if (OpSTR == "!") {
            return TOKENS::OPERATORS::LOGIC::NOT_{};
        }
        if (OpSTR == "=") {
            return TOKENS::OPERATORS::AS{};
        }
        if (OpSTR == "==") {
            return TOKENS::OPERATORS::COMP::E{};
        }
        if (OpSTR == "!=") {
            return TOKENS::OPERATORS::COMP::NE{};
        }
        if (OpSTR == ">=") {
            return TOKENS::OPERATORS::COMP::GE{};
        }
        if (OpSTR == "<=") {
            return TOKENS::OPERATORS::COMP::LE{};
        }
        if (OpSTR == "->") {
            return TOKENS::ARROW{};
        }
        if (OpSTR == "-") {
            return TOKENS::OPERATORS::ARITHMETICS::MN{};
        }
//        ++current;
    }

    while ((isdigit(LastChar) || (isIn(LastChar, ".")))
           && (IdentifierString == "")) {
        NumStr += LastChar;
        LastChar = *current;
        ++current;
    }
    if ((IdentifierString == "") && (NumStr != "")) {
        if (isIn('.', NumStr)) {
            double n = strtod(NumStr.c_str(), 0);
            token t = TOKENS::DOUBLE{n};
            return t;
        } else {
            int n = strtol(NumStr.c_str(), 0, 10);
            token t = TOKENS::INT{n};
            return t;
        }
    }

    if (isalpha(LastChar) || (LastChar == '_')) {
        while (isalnum(LastChar) || (LastChar == '_')) {
            IdentifierString += LastChar;
            LastChar = *current;
            if (isalnum(LastChar) || (LastChar == '_')) {
                ++current;
            }
        }
        if (IdentifierString == "def") {
            cout << "DEF_FUN" << endl;
            return TOKENS::DEF_FUN{};
        }
        if (IdentifierString == "var") {
            cout << "DEF_VAR" << endl;
            return TOKENS::DEF_VAR{};
        }
//        if (Identifier_string == "int") {
//            return TOKENS::IDENTIFIER{};
//        }
//        if (Identifier_string == "double") {
//            return TOKENS::IDENTIFIER{};
//        }
//        if (Identifier_string == "bool") {
//            return TOKENS::IDENTIFIER{};
//        }
        if (IdentifierString == "true") {
            return TOKENS::BOOL{true};
        }
        if (IdentifierString == "false") {
            return TOKENS::BOOL{false};
        }
        if (IdentifierString == "return") {
            cout << "RETURN" << endl;
            return TOKENS::RETURN{};
        }
        if (IdentifierString == "if") {
            cout << "IF" << endl;
            return TOKENS::IF{};
        }
        if (IdentifierString == "else") {
            cout << "ELSE" << endl;
            return TOKENS::ELSE{};
        }
        if (IdentifierString == "do") {
            cout << "DO" << endl;
            return TOKENS::DO{};
        }
        if (IdentifierString == "while") {
            cout << "WHILE" << endl;
            return TOKENS::WHILE{};
        }
        cout << "NAME_OR_TYPE_IDENTIFIER" << endl;
        return TOKENS::IDENTIFIER{IdentifierString};
    }
}

string open_file(string file_name) {
    std::string src;
    {
        ifstream code(file_name);
        if (!code) {
            cout << "не открылся" << endl;
        }
        src.assign(std::istreambuf_iterator<char>{code},
                      std::istreambuf_iterator<char>{});
    }
    return src;
}

std::vector<token> get_tokens(string s) {
    string::iterator current = s.begin();
    std::vector<token> vec{};
    while(current != s.end()){
        token t = get_token(current, s.end());
        vec.push_back(t);
    }
    return vec;
}

//int main() {
//    // получение массива с токенами.
//    fprintf(stderr, "ready> ");

//    std::string src;
//    std::string file_name = "code.txt";
//    src = open_file(file_name);

//    std::vector<token> vec{};
//    vec = get_tokens(src);

//    // модуль который содержит весь код.
//    TheModule = llvm::make_unique<Module>("my cool jit", TheContext);

//    // Run the main "interpreter loop" now.
//    MainLoop();

//    // Вывод сгенерированного кода.
//    TheModule->print(errs(), nullptr);

//    return 0;
//}
