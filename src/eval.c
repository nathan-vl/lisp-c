#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eval.h"

bool isNumber(Object *object)
{
    if (object == NULL)
    {
        return false;
    }
    if (object->kind == IDENTIFIER)
    {
        // TODO: Checar valor da variável se é número
        return false;
    }
    if (object->kind != NUMBER)
    {
        return false;
    }
    return true;
}

double getNumber(Object *object)
{
    Object eval = evaluate(*object);
    if (!isNumber(&eval))
    {
        printf("Error. Expression should eval to number type.\n");
        exit(-1);
    }
    return eval.value.number;
}

Object add(List *list)
{
    double result = 0;
    while (list != NULL)
    {
        Object value = evaluate(list->car);
        double number = getNumber(&value);
        result += number;
        list = list->cdr;
    }
    return numberObject(result);
}

Object subtract(List *list)
{
    Object eval = evaluate(list->car);
    double result = getNumber(&eval);
    list = list->cdr;
    while (list != NULL)
    {
        Object value = evaluate(list->car);
        double number = getNumber(&value);
        result -= number;
        list = list->cdr;
    }
    return numberObject(result);
}

Object multiply(List *list)
{
    double result = 1;
    while (list != NULL)
    {
        Object value = evaluate(list->car);
        double number = getNumber(&value);
        result *= number;
        list = list->cdr;
    }
    return numberObject(result);
}

Object divide(List *list)
{
    Object eval = evaluate(list->car);
    double result = getNumber(&eval);
    list = list->cdr;
    while (list != NULL)
    {
        Object value = evaluate(list->car);
        double number = getNumber(&value);
        result /= number;
        list = list->cdr;
    }
    return numberObject(result);
}

Object evaluateList(List *list)
{
    if (list == NULL)
    {
        // ?
    }

    Object car = list->car;
    if (car.kind == LIST)
    {
        printf("Error. Could not call list\n");
        exit(-1);
    }

    if (car.kind != IDENTIFIER)
    {
        printf("Error. Could not call literal\n");
        exit(-1);
    }

    char *identifier = car.value.identifier;
    if (strcmp(identifier, "+") == 0)
        return add(list->cdr);

    if (strcmp(identifier, "-") == 0)
        return subtract(list->cdr);

    if (strcmp(identifier, "*") == 0)
        return multiply(list->cdr);

    if (strcmp(identifier, "/") == 0)
        return divide(list->cdr);
}

Object evaluate(Object object)
{
    if (object.kind == LIST)
    {
        List *list = object.value.list;
        return evaluateList(list);
    }

    if (object.kind == IDENTIFIER)
    {
    }

    if (object.kind != LIST && object.kind != IDENTIFIER)
    {
        return object;
    }

    // printf("Error. Identifier \"%s\" not detected\n", list->car.value.atom.value.identifier);
}