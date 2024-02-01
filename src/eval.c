#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eval.h"

bool isTruthy(Environment *env, Object *object)
{
    Object eval = evaluate(env, *object);
    return eval.kind != BOOLEAN || eval.value.boolean;
}

bool isNumber(Environment *env, Object *object)
{
    if (object == NULL)
    {
        return false;
    }

    if (object->kind == IDENTIFIER)
    {
        Object eval = evaluate(env, *object);
        return isNumber(env, &eval);
    }

    return object->kind == NUMBER;
}

double getNumber(Environment *env, Object *object)
{
    Object eval = evaluate(env, *object);
    if (!isNumber(env, &eval))
    {
        printf("Error. Expression should eval to number type.\n");
        exit(-1);
    }
    return eval.value.number;
}

Object define(Environment *env, List *list)
{
    if (list->car.kind != IDENTIFIER)
    {
        printf("Error. Expected identifier.\n");
        exit(-1);
    }

    if (list->cdr == NULL)
    {
        printf("Error. Expected identifier value.\n");
        exit(-1);
    }

    if (list->cdr->cdr != NULL)
    {
        printf("Error. Expected exactly 2 arguments.\n");
        exit(-1);
    }

    char *identifier = list->car.value.identifier;
    defineVariable(env, identifier, &list->cdr->car);
    return booleanObject(true);
}

Object add(Environment *env, List *list)
{
    double result = 0;
    while (list != NULL)
    {
        Object value = evaluate(env, list->car);
        double number = getNumber(env, &value);
        result += number;
        list = list->cdr;
    }
    return numberObject(result);
}

Object subtract(Environment *env, List *list)
{
    Object eval = evaluate(env, list->car);
    double result = getNumber(env, &eval);
    list = list->cdr;
    while (list != NULL)
    {
        Object value = evaluate(env, list->car);
        double number = getNumber(env, &value);
        result -= number;
        list = list->cdr;
    }
    return numberObject(result);
}

Object multiply(Environment *env, List *list)
{
    double result = 1;
    while (list != NULL)
    {
        Object value = evaluate(env, list->car);
        double number = getNumber(env, &value);
        result *= number;
        list = list->cdr;
    }
    return numberObject(result);
}

Object divide(Environment *env, List *list)
{
    Object eval = evaluate(env, list->car);
    double result = getNumber(env, &eval);
    list = list->cdr;
    while (list != NULL)
    {
        Object value = evaluate(env, list->car);
        double number = getNumber(env, &value);
        result /= number;
        list = list->cdr;
    }
    return numberObject(result);
}

Object negation(Environment *env, List *list)
{
    if (list->cdr != NULL)
    {
        printf("Error. Function \"not\" expects one argument, but given more.\n");
        exit(-1);
    }

    return booleanObject(!isTruthy(env, &list->car));
}

Object evaluateList(Environment *env, List *list)
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
    if (strcmp(identifier, "define") == 0)
        return define(env, list->cdr);

    if (strcmp(identifier, "+") == 0)
        return add(env, list->cdr);
    if (strcmp(identifier, "-") == 0)
        return subtract(env, list->cdr);
    if (strcmp(identifier, "*") == 0)
        return multiply(env, list->cdr);
    if (strcmp(identifier, "/") == 0)
        return divide(env, list->cdr);

    if (strcmp(identifier, "not") == 0)
        return negation(env, list->cdr);
}

Object evaluate(Environment *env, Object object)
{
    if (object.kind == LIST)
    {
        List *list = object.value.list;
        return evaluateList(env, list);
    }

    if (object.kind == IDENTIFIER)
    {
        char *identifier = object.value.identifier;
        Object *var = getVariable(env, identifier);
        if (var == NULL)
        {
            printf("Error. Variable \"%s\" is not defined.\n", identifier);
            exit(-1);
        }
        return evaluate(env, *var);
    }

    return object;
}