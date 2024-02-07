#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "scanner.h"

struct SyntaxAnalyserStatus
{
    struct TokenLinkedList *current;
};

struct Expression parseExpression(struct SyntaxAnalyserStatus *status);

struct Pair *newPair(struct Expression car, struct Expression cdr)
{
    struct Pair *pair = malloc(sizeof(struct Pair));
    pair->car = car;
    pair->cdr = cdr;
    return pair;
}

struct Expression newPairExpression(struct Expression car, struct Expression cdr)
{
    return pairExpression(newPair(car, cdr));
}

struct Pair *parsePair(struct SyntaxAnalyserStatus *status)
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

    struct Pair *pair = newPair(parseExpression(status), pairExpression(NULL));

    struct Pair *current = pair;
    while (status->current->token.type != T_CLOSE_PAREN)
    {
        current->cdr = newPairExpression(parseExpression(status), pairExpression(NULL));
        current = current->cdr.value.pair;
    }
    status->current = status->current->next;

    return pair;
}

struct Expression parseQuote(struct SyntaxAnalyserStatus *status)
{
    struct Expression car = identifierExpression("quote");

    struct Expression obj = parseExpression(status);
    struct Expression cdr = newPairExpression(obj, pairExpression(NULL));

    return newPairExpression(car, cdr);
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
        return pairExpression(parsePair(status));
    case T_F:
        return booleanExpression(false);
    case T_T:
        return booleanExpression(true);
    case T_CHARACTER:
        return characterExpression(token.literal.value.character);
    case T_IDENTIFIER:
        return identifierExpression(token.lexeme);
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
