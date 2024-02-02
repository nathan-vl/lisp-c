#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

typedef struct SyntaxAnalyserStatus
{
    TokenLinkedList *current;
} SyntaxAnalyserStatus;

Object parseObject(SyntaxAnalyserStatus *status);

List *parseList(SyntaxAnalyserStatus *status)
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

    List *list = malloc(sizeof(List));
    list->car = parseObject(status);

    List *current = list;
    while (status->current->token.type != T_CLOSE_PAREN)
    {
        current->cdr = malloc(sizeof(List));
        current->cdr->car = parseObject(status);
        current = current->cdr;
    }
    current->cdr = NULL;
    status->current = status->current->next;

    return list;
}

Object parseQuote(SyntaxAnalyserStatus *status)
{
    List *list = malloc(sizeof(List));
    list->car = identifierObject("quote");
    list->cdr = malloc(sizeof(List));
    list->cdr->car = parseObject(status);
    list->cdr->cdr = NULL;
    return listObject(list);
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
        return listObject(parseList(status));
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
        // Error
        break;
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
