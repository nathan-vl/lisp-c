#pragma once

#include "scanner.h"
#include "object.h"

struct ObjectLinkedList
{
    struct Object value;
    struct ObjectLinkedList *next;
};


struct ObjectLinkedList *syntaxAnalyser(struct TokenLinkedList *tokens);
