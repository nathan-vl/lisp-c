#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "scanner.h"

struct SyntaxAnalyserStatus
{
    struct TokenLinkedList *current;
};

struct Object parseObject(struct SyntaxAnalyserStatus *status);

struct Pair *newPair(struct Object car, struct Object cdr)
{
    struct Pair *pair = malloc(sizeof(struct Pair));
    pair->car = car;
    pair->cdr = cdr;
    return pair;
}

struct Object newPairObject(struct Object car, struct Object cdr)
{
    return pairObject(newPair(car, cdr));
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

    struct Pair *pair = newPair(parseObject(status), pairObject(NULL));

    struct Pair *current = pair;
    while (status->current->token.type != T_CLOSE_PAREN)
    {
        current->cdr = newPairObject(parseObject(status), pairObject(NULL));
        current = current->cdr.value.pair;
    }
    status->current = status->current->next;

    return pair;
}

struct Object parseQuote(struct SyntaxAnalyserStatus *status)
{
    struct Object car = identifierObject("quote");

    struct Object obj = parseObject(status);
    struct Object cdr = newPairObject(obj, pairObject(NULL));

    return newPairObject(car, cdr);
}

struct Object parseObject(struct SyntaxAnalyserStatus *status)
{
    struct Token token = status->current->token;
    status->current = status->current->next;
    switch (token.type)
    {
    case T_APOSTROPHE:
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

struct ObjectLinkedList *syntaxAnalyser(struct TokenLinkedList *tokens)
{
    struct SyntaxAnalyserStatus status;
    status.current = tokens;

    struct ObjectLinkedList objects;
    struct ObjectLinkedList *current = &objects;

    while (status.current != NULL)
    {
        current->next = malloc(sizeof(struct ObjectLinkedList));
        current = current->next;
        current->value = parseObject(&status);
        current->next = NULL;
    }

    return objects.next;
}
