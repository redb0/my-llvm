#ifndef _TOKENS_H_
#define _TOKENS_H_


#include <string>
#include "mpark/variant.hpp"

using mpark::holds_alternative;
using mpark::get_if;
using mpark::visit;
using mpark::swap;
using mpark::bad_variant_access;
using mpark::variant;

struct TOKENS {
    struct END_OF_FILE {};
    struct DEF_FUN {};
    struct ARROW {};     // ->
    struct IDENTIFIER {std::string name;};
    // struct NUMERIC {};
    struct OPEN_PAREN {};

    struct CLOSE_PAREN {};
    struct COMMA {};

    struct OPEN_BRACES {};
    struct CLOSE_BRACES {};

    struct IF {};
    struct ELSE {};
    struct DO {};
    struct WHILE {};

    struct DEF_VAR {};

    struct BOOL {bool var;};
    // struct TRUE_ {};
    // struct FALSE_ {};

    struct INT {int var;};
    struct DOUBLE {double var;};

    struct DEF_STRUCT {};
    struct RETURN {};

    // struct  { constexpr int var = ;};

    struct OPERATORS {
        struct COMP {
            struct E {constexpr static int priority = 9;};  // ==
            struct NE {constexpr static int priority = 9;}; // !==
            struct G {constexpr static int priority = 8;};  // >
            struct L {constexpr static int priority = 8;};  // <
            struct GE {constexpr static int priority = 8;}; // >=
            struct LE {constexpr static int priority = 8;}; // <=
        };
        struct ARITHMETICS {
            struct UP {constexpr static int priority = 3;};  // + не добавлен
            struct UMN {constexpr static int priority = 3;};  // - не добавлен

            struct P {constexpr static int priority = 6;};  // +
            struct MN {constexpr static int priority = 6;}; // -
            struct MP {constexpr static int priority = 5;}; // *
            struct D {constexpr static int priority = 5;};  // /
        };
        struct LOGIC {
            struct AND_ {constexpr static int priority = 13;}; // &&
            struct OR_ {constexpr static int priority = 14;};  // ||
            struct NOT_ {constexpr static int priority = 3;}; // !
        };
        struct AS {constexpr static int priority = 15;}; // =
    };
    struct SEMICOLON {};   // ;
    struct END_OF_LINE {}; // \n
    struct COMMENT {};     // #

    using token = variant<END_OF_FILE,
        DEF_FUN, IDENTIFIER, OPEN_PAREN,
        CLOSE_PAREN, COMMA, OPEN_BRACES,CLOSE_BRACES,
        IF, ELSE, DEF_VAR, BOOL, INT, DOUBLE, SEMICOLON, END_OF_LINE,
        RETURN, DO, WHILE, ARROW,

        OPERATORS::COMP::E, OPERATORS::COMP::NE, OPERATORS::COMP::G,
        OPERATORS::COMP::L, OPERATORS::COMP::GE, OPERATORS::COMP::LE,

        OPERATORS::ARITHMETICS::P, OPERATORS::ARITHMETICS::MN,
        OPERATORS::ARITHMETICS::MP, OPERATORS::ARITHMETICS::D,
        OPERATORS::ARITHMETICS::UP, OPERATORS::ARITHMETICS::UMN,

        OPERATORS::LOGIC::AND_, OPERATORS::LOGIC::OR_,
        OPERATORS::LOGIC::NOT_,

        OPERATORS::AS>;
};

#endif //_TOKENS_H_
