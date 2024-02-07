#pragma once

#include "token.h"

struct TokenLinkedList
{
    struct Token token;
    struct TokenLinkedList *next;
};

struct TokenLinkedList *parse(char *source);
void freeToken(struct Token token);
