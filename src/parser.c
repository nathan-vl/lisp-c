#include <stdio.h>
#include <stdlib.h>

#include "atom.h"
#include "object.h"
#include "lexer.h"

typedef struct SyntaxAnalyserStatus
{
    TokenLinkedList *current;
} SyntaxAnalyserStatus;

Object consToObject(Cons *cons)
{
    Object object;
    object.kind = CONS;
    object.value.cons = cons;
    return object;
}

Object atomToObject(Atom atom)
{
    Object object;
    object.kind = ATOM;
    object.value.atom = atom;
    return object;
}

Atom parseAtom(SyntaxAnalyserStatus *status)
{
    Token token = status->current->token;
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
    Cons *cons = NULL;

    if (status->current == NULL)
    {
        printf("Error. Expected closing ')'\n");
        exit(-1);
    }

    switch (status->current->token.type)
    {
    case T_OPEN_PAREN:
        cons = malloc(sizeof(Cons));
        status->current = status->current->next;
        Cons *innerCons = parseCons(status);
        cons->car = consToObject(innerCons);
        break;
    case T_CLOSE_PAREN:
        break;
    default:
        cons = malloc(sizeof(Cons));
        Atom atom = parseAtom(status);
        cons->car = atomToObject(atom);
        status->current = status->current->next;
    }

    Cons *current = cons;
    while (status->current->token.type != T_CLOSE_PAREN)
    {
        current->cdr = malloc(sizeof(Cons));
        current->cdr->car = parseObject(status);
        current = current->cdr;

        status->current = status->current->next;
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
        status->current = status->current->next;
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
