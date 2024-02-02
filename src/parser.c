#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

typedef struct SyntaxAnalyserStatus
{
    TokenLinkedList *current;
} SyntaxAnalyserStatus;

Object parseObject(SyntaxAnalyserStatus *status);

Pair *newPair(Object car, Object cdr)
{
    Pair *pair = malloc(sizeof(Pair));
    pair->car = car;
    pair->cdr = cdr;
    return pair;
}

Object newPairObject(Object car, Object cdr)
{
    return pairObject(newPair(car, cdr));
}

Pair *parsePair(SyntaxAnalyserStatus *status)
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

    Pair *pair = newPair(parseObject(status), pairObject(NULL));

    Pair *current = pair;
    while (status->current->token.type != T_CLOSE_PAREN)
    {
        current->cdr = newPairObject(parseObject(status), pairObject(NULL));
        current = current->cdr.value.pair;
    }
    status->current = status->current->next;

    return pair;
}

Object parseQuote(SyntaxAnalyserStatus *status)
{
    Object car = identifierObject("quote");

    Object obj = parseObject(status);
    Object cdr = newPairObject(obj, pairObject(NULL));

    return newPairObject(car, cdr);
}

Object parseObject(SyntaxAnalyserStatus *status)
{
    Token token = status->current->token;
    status->current = status->current->next;
    switch (token.type)
    {
    case T_QUOTE:
        return parseQuote(status);
    case T_OPEN_PAREN:
        return pairObject(parsePair(status));
    case T_F:
        return booleanObject(false);
    case T_T:
        return booleanObject(true);
    case T_CHARACTER:
        return characterObject(token.literal.value.character);
    case T_IDENTIFIER:
        return identifierObject(token.lexeme);
    case T_NUMBER:
        return numberObject(token.literal.value.number);
    case T_STRING:
        return stringObject(token.literal.value.string);
    default:
        printf("Error. Unrecognized token.\n");
        exit(-1);
    }
}

ObjectLinkedList *syntaxAnalyser(TokenLinkedList *tokens)
{
    SyntaxAnalyserStatus status;
    status.current = tokens;

    ObjectLinkedList objects;
    ObjectLinkedList *current = &objects;

    while (status.current != NULL)
    {
        current->next = malloc(sizeof(ObjectLinkedList));
        current = current->next;
        current->value = parseObject(&status);
        current->next = NULL;
    }

    return objects.next;
}
