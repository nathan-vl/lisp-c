#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eval.h"

void printList(List *list)
{
    printf("(");
    if (list != NULL)
    {
        printObject(&list->car);

        list = list->cdr;
        while (list != NULL)
        {
            printf(" ");
            printObject(&list->car);
            list = list->cdr;
        }
    }
    printf(")");
}

void printObject(Object *object)
{
    switch (object->kind)
    {
    case LIST:
        printList(object->value.list);
        break;
    case BOOLEAN:
        printf("%s", object->value.boolean ? "#t" : "#f");
        break;
    case CHARACTER:
        printf("%c", object->value.character);
        break;
    case IDENTIFIER:
        printf("%s", object->value.identifier);
        break;
    case NUMBER:
        printf("%f", object->value.number);
        break;
    case PROCEDURE:
        printf("<proc>");
        break;
    case STRING:
        printf("\"%s\"", object->value.string);
        break;
    }
}

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

Object print(Environment *env, List *list)
{
    Object car = evaluate(env, list->car);
    printObject(&car);

    list = list->cdr;
    while (list != NULL)
    {
        printf(" ");
        car = evaluate(env, list->car);
        printObject(&car);
        list = list->cdr;
    }
    printf("\n");
    return booleanObject(true);
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

Object quote(Environment *env, List *list)
{
    return list->car;
}

// TODO: Add enclosing environments to environments
Object lambda(Environment *env, List *list)
{
    // TODO: Check if args are all symbols
    if (list->car.kind != LIST)
    {
        printf("Error. Args should be a list.\n");
        exit(-1);
    }

    // TODO: Check if params are only args and body
    if (list->cdr->car.kind != LIST)
    {
        printf("Error. Body should be a list.\n");
        exit(-1);
    }

    Procedure procedure;
    procedure.parameters = list->car.value.list;
    procedure.body = list->cdr->car.value.list;
    return procedureObject(procedure);
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

size_t listLength(List *list)
{
    size_t length = 0;
    while (list != NULL)
    {
        length++;
        list = list->cdr;
    }
    return length;
}

Object executeProcedure(Environment *env, Procedure procedure, List *args)
{
    size_t paramsLength = listLength(procedure.parameters);
    size_t argsLength = listLength(args);

    if (paramsLength != argsLength)
    {
        printf("Error. Arity difference in procedure. Expected %ld but got %ld.\n", paramsLength, argsLength);
        exit(-1);
    }

    Environment innerEnv;
    List *parameters = procedure.parameters;
    while (args != NULL)
    {
        defineVariable(&innerEnv, parameters->car.value.identifier, &args->car);
        parameters = parameters->cdr;
        args = args->cdr;
    }

    return evaluate(&innerEnv, listObject(procedure.body));
}

Object evaluateList(Environment *env, List *list)
{
    if (list == NULL)
    {
        return listObject(list);
    }

    Object car = list->car;

    // Check for default procedures
    // TODO: Research for if identifiers should be tokens for performance and cleaner evaluation
    if (car.kind == IDENTIFIER)
    {
        char *identifier = car.value.identifier;
        if (strcmp(identifier, "define") == 0)
            return define(env, list->cdr);
        if (strcmp(identifier, "lambda") == 0)
            return lambda(env, list->cdr);
        if (strcmp(identifier, "print") == 0)
            return print(env, list->cdr);
        if (strcmp(identifier, "quote") == 0)
            return quote(env, list->cdr);

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

    car = evaluate(env, list->car);
    if (car.kind == PROCEDURE)
    {
        Procedure procedure = car.value.procedure;
        return executeProcedure(env, procedure, list->cdr);
    }

    printf("Error. Could not call value\n");
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