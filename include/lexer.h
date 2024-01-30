#ifndef LEXER_H
#define LEXER_H

#include "literal.h"

typedef enum TokenType
{
    // False (nil) and true (t)
    T_NIL,
    T_T,

    // Parentheses
    T_OPEN_PAREN,
    T_CLOSE_PAREN,

    // Reserved terms
    T_DEFINE,
    T_FOR,
    T_IF,
    T_LAMBDA,
    T_LET,
    T_LIST,
    T_QUOTE,
    T_WHILE,

    // Variables, functions and literals
    T_IDENTIFIER,
    T_STRING,
    T_NUMBER,

    T_WHITESPACE,
    T_EOF,
} TokenType;

typedef struct LexerStatus
{
    char *source;
    int start;
    int current;
} LexerStatus;

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
void printToken(Token token);
void freeToken(Token token);

#endif
