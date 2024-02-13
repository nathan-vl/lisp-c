#pragma once

#include <stddef.h>
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

    size_t line;
    size_t col;
};

struct Token newToken(enum TokenType type, char *lexeme, size_t line, size_t col);
struct Token newStringToken(char *string, size_t line, size_t col);
struct Token newNumberToken(char *strNumber, size_t line, size_t col);
