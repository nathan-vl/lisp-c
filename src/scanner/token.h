#pragma once

#include "literal.h"

enum TokenType
{
    // False (f) and true (t)
    T_F,
    T_T,

    // Parentheses and apostrophe (')
    T_OPEN_PAREN,
    T_CLOSE_PAREN,
    T_APOSTROPHE,

    // Symbols and literals
    T_CHARACTER,
    T_SYMBOL,
    T_NUMBER,
    T_STRING,
};

struct Token
{
    enum TokenType type;
    char *lexeme;
    struct Literal literal;
};

struct Token newToken(enum TokenType type, char *lexeme);
struct Token newStringToken(char *string);
struct Token newNumberToken(char *strNumber);
