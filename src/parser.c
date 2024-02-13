#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "scanner.h"

struct SyntaxAnalyserStatus
{
    struct TokenLinkedList *current;
};

struct Expression parseExpression(struct SyntaxAnalyserStatus *status);

struct List *newList(struct Expression car, struct List *cdr)
{
    struct List *list = malloc(sizeof(struct List));
    list->car = car;
    list->cdr = cdr;
    return list;
}

struct Expression newListExpression(struct Expression car, struct List *cdr)
{
    return listExpression(newList(car, cdr));
}

struct List *parseList(struct SyntaxAnalyserStatus *status)
{
    if (status->current == NULL)
    {
        printf("Error. Expected closing ')'\n");
        exit(-1);
    }

    if (status->current->token.type == T_CLOSE_PAREN)
    {
        status->current = status->current->next;
        return NULL;
    }

    struct List *list = newList(parseExpression(status), NULL);

    struct List *current = list;
    while (status->current->token.type != T_CLOSE_PAREN)
    {
        current->cdr = newList(parseExpression(status), NULL);
        current = current->cdr;
    }
    status->current = status->current->next;

    return list;
}

struct Expression parseQuote(struct SyntaxAnalyserStatus *status)
{
    struct Expression car = symbolExpression("quote");

    struct List *cdr = newList(parseExpression(status), NULL);

    return newListExpression(car, cdr);
}

struct Expression parseExpression(struct SyntaxAnalyserStatus *status)
{
    struct Token token = status->current->token;
    status->current = status->current->next;
    switch (token.type)
    {
    case T_APOSTROPHE:
        return parseQuote(status);
    case T_OPEN_PAREN:
        return listExpression(parseList(status));
    case T_F:
        return booleanExpression(false);
    case T_T:
        return booleanExpression(true);
    case T_CHARACTER:
        return characterExpression(token.literal.value.character);
    case T_SYMBOL:
        return symbolExpression(token.lexeme);
    case T_NUMBER:
        return numberExpression(token.literal.value.number);
    case T_STRING:
        return stringExpression(token.literal.value.string);
    default:
        printf("Error. Unrecognized token.\n");
        exit(-1);
    }
}

struct ExpressionLinkedList *syntaxAnalyser(struct TokenLinkedList *tokens)
{
    struct SyntaxAnalyserStatus status;
    status.current = tokens;

    struct ExpressionLinkedList expressions;
    struct ExpressionLinkedList *current = &expressions;

    while (status.current != NULL)
    {
        current->next = malloc(sizeof(struct ExpressionLinkedList));
        current = current->next;
        current->value = parseExpression(&status);
        current->next = NULL;
    }

    return expressions.next;
}
