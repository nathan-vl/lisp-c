#include <stdio.h>
#include <stdlib.h>

#include "object.h"
#include "lexer.h"

typedef struct SyntaxAnalyserStatus
{
    TokenLinkedList *current;
} SyntaxAnalyserStatus;

Object parseValue(SyntaxAnalyserStatus *status)
{
    Token token = status->current->token;
    status->current = status->current->next;
    switch (token.type)
    {
    case T_F:
        return booleanObject(false);
    case T_T:
        return booleanObject(true);
    // case T_CHARACTER:
    //     return characterObject(token.literal.value.character);
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

Object parseObject(SyntaxAnalyserStatus *status);

List *parseList(SyntaxAnalyserStatus *status)
{
    status->current = status->current->next;
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

Object parseObject(SyntaxAnalyserStatus *status)
{
    Token token = status->current->token;
    switch (token.type)
    {
    case T_OPEN_PAREN:
        return listObject(parseList(status));
    default:
        return parseValue(status);
    }
}

Object syntaxAnalyser(TokenLinkedList *tokens)
{
    SyntaxAnalyserStatus status;
    status.current = tokens;

    while (status.current != NULL)
    {
        return parseObject(&status);
    }
}
