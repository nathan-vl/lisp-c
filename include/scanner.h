#ifndef SCANNER_H
#define SCANNER_H

#include "literal.h"

typedef enum TokenType
{
    // False (f) and true (t)
    T_F,
    T_T,

    // Parentheses
    T_OPEN_PAREN,
    T_CLOSE_PAREN,

    // Identifiers and literals
    T_CHARACTER,
    T_IDENTIFIER,
    T_NUMBER,
    T_STRING,

    T_WHITESPACE,
    T_EOF,
} TokenType;

typedef struct ScannerStatus
{
    char *source;
    int line;
    int col;
} ScannerStatus;

typedef struct Token
{
    TokenType type;
    char *lexeme;
    Literal literal;
} Token;

typedef struct TokenLinkedList
{
    Token token;
    struct TokenLinkedList *next;
} TokenLinkedList;

TokenLinkedList *parse(char *source);
void freeToken(Token token);

#endif
