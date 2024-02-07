#pragma once

#include "scanner.h"
#include "expression.h"

struct ExpressionLinkedList
{
    struct Expression value;
    struct ExpressionLinkedList *next;
};

struct ExpressionLinkedList *syntaxAnalyser(struct TokenLinkedList *tokens);
