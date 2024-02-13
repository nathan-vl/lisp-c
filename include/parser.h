#pragma once

#include "scanner.h"
#include "expression.h"

struct SyntaxAnalyserStatus
{
    bool hasError;
    size_t line;
    size_t col;

    struct TokenLinkedList *current;
    struct ExpressionLinkedList *expressions;
};

struct ExpressionLinkedList
{
    struct Expression value;
    struct ExpressionLinkedList *next;
};

struct SyntaxAnalyserStatus syntaxAnalyser(struct TokenLinkedList *tokens);
