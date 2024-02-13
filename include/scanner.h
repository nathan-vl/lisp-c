#pragma once

#include "token.h"

struct TokenLinkedList
{
    struct Token token;
    struct TokenLinkedList *next;
};

struct ScannerStatus
{
    bool hasError;
    char *source;
    size_t line;
    size_t col;

    struct TokenLinkedList tokensHead;
    struct TokenLinkedList *currentToken;
};

struct ScannerStatus parse(char *source);
void freeToken(struct Token token);
