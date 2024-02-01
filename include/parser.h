#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "object.h"

typedef struct ObjectLinkedList
{
    Object value;
    struct ObjectLinkedList *next;
} ObjectLinkedList;

ObjectLinkedList *syntaxAnalyser(TokenLinkedList *tokens);

#endif
