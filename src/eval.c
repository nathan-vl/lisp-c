#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eval.h"

Literal addValues(Cons *cons)
{
    double result = 0;
    while (cons != NULL)
    {
        result += cons->car.value.atom.value.literal.value.number;
        cons = cons->cdr;
    }
    return newNumberLiteral(result);
}

Object evaluate(Object object)
{
    if (object.kind == ATOM)
    {
        return object;
    }

    Cons *cons = object.value.cons;
    if (cons == NULL)
    {
        return atomToObject(literalToAtom(newNilLiteral()));
    }

    Object car = cons->car;
    if (car.kind == CONS)
    {
        printf("Error. Could not call cons\n");
        exit(-1);
    }

    Atom atom = car.value.atom;
    if (atom.kind == LITERAL)
    {
        printf("Error. Could not call literal\n");
        exit(-1);
    }

    char *identifier = atom.value.identifier;
    if (strcmp(identifier, "+") == 0)
    {
        Literal result = addValues(cons->cdr);
        return atomToObject(literalToAtom(result));
    }

    printf("Error. Identifier \"%s\" not detected\n", cons->car.value.atom.value.identifier);
}