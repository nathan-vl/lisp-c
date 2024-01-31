#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eval.h"

bool isNumber(Object *object)
{
    if (object == NULL || object->kind == CONS)
    {
        return false;
    }
    if (object->value.atom.kind == IDENTIFIER)
    {
        // TODO: Checar valor da variável se é número
        return false;
    }
    if (object->value.atom.value.literal.kind != NUMBER)
    {
        return false;
    }
    return true;
}

double getNumber(Object *object)
{
    if (!isNumber(object))
    {
        printf("Error. Expression should eval to number type.\n");
        exit(-1);
    }
    return object->value.atom.value.literal.value.number;
}

Literal add(Cons *cons)
{
    double result = 0;
    while (cons != NULL)
    {
        Object value = evaluate(cons->car);
        double number = getNumber(&value);
        result += number;
        cons = cons->cdr;
    }
    return newNumberLiteral(result);
}

Literal subtract(Cons *cons)
{
    Object eval = evaluate(cons->car);
    double result = getNumber(&eval);
    cons = cons->cdr;
    while (cons != NULL)
    {
        Object value = evaluate(cons->car);
        double number = getNumber(&value);
        result -= number;
        cons = cons->cdr;
    }
    return newNumberLiteral(result);
}

Literal multiply(Cons *cons)
{
    double result = 1;
    while (cons != NULL)
    {
        Object value = evaluate(cons->car);
        double number = getNumber(&value);
        result *= number;
        cons = cons->cdr;
    }
    return newNumberLiteral(result);
}

Literal divide(Cons *cons)
{
    Object eval = evaluate(cons->car);
    double result = getNumber(&eval);
    cons = cons->cdr;
    while (cons != NULL)
    {
        Object value = evaluate(cons->car);
        double number = getNumber(&value);
        result /= number;
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
        Literal result = add(cons->cdr);
        return atomToObject(literalToAtom(result));
    }
    else if (strcmp(identifier, "-") == 0)
    {
        Literal result = subtract(cons->cdr);
        return atomToObject(literalToAtom(result));
    }
    else if (strcmp(identifier, "*") == 0)
    {
        Literal result = multiply(cons->cdr);
        return atomToObject(literalToAtom(result));
    }
    else if (strcmp(identifier, "/") == 0)
    {
        Literal result = divide(cons->cdr);
        return atomToObject(literalToAtom(result));
    }

    printf("Error. Identifier \"%s\" not detected\n", cons->car.value.atom.value.identifier);
}