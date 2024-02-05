#ifndef DATA_TYPES_H
#define DATA_TYPES_H

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

    // Identifiers and literals
    T_CHARACTER,
    T_IDENTIFIER,
    T_NUMBER,
    T_STRING,

    T_WHITESPACE,
    T_EOF,
};

struct ScannerStatus
{
    char *source;
    int line;
    int col;
};

struct Token
{
    enum TokenType type;
    char *lexeme;
    struct Literal literal;
};

struct TokenLinkedList
{
    struct Token token;
    struct TokenLinkedList *next;
};

#endif
