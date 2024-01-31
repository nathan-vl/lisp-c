#include <stdio.h>
#include <stdlib.h>

#include "atom.h"
#include "object.h"
#include "lexer.h"

typedef struct SyntaxAnalyserStatus
{
    TokenLinkedList *current;
} SyntaxAnalyserStatus;

Atom parseAtom(SyntaxAnalyserStatus *status)
{
    Token token = status->current->token;
    status->current = status->current->next;
    switch (token.type)
    {
    case T_NIL:
    case T_T:
    case T_STRING:
    case T_NUMBER:
        return literalToAtom(token.literal);
    default:
        return identifierToAtom(token.lexeme);
    }
}

Object parseObject(SyntaxAnalyserStatus *status);

Cons *parseCons(SyntaxAnalyserStatus *status)
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

    Cons *cons = malloc(sizeof(Cons));
    cons->car = parseObject(status);

    Cons *current = cons;
    while (status->current->token.type != T_CLOSE_PAREN)
    {
        current->cdr = malloc(sizeof(Cons));
        current->cdr->car = parseObject(status);
        current = current->cdr;
    }
    current->cdr = NULL;
    status->current = status->current->next;

    return cons;
}

Object parseObject(SyntaxAnalyserStatus *status)
{
    Token token = status->current->token;
    switch (token.type)
    {
    case T_OPEN_PAREN:
        return consToObject(parseCons(status));
    default:
        return atomToObject(parseAtom(status));
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
