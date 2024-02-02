#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eval.h"

bool isList(Object *object)
{
    return (object->kind == PAIR) && (object->value.pair == NULL || isList(&object->value.pair->cdr));
}

bool isPair(Object *object)
{
    return (object->kind == PAIR) && (object->value.pair != NULL) && !isList(&object->value.pair->cdr);
}

void printPair(Pair *pair)
{
    printf("(");
    printObject(&pair->car);
    printf(" . ");
    printObject(&pair->cdr);
    printf(")");
}

void printList(Pair *list)
{
    printf("(");
    if (list != NULL)
    {
        printObject(&list->car);

        list = list->cdr.value.pair;
        while (list != NULL)
        {
            printf(" ");
            printObject(&list->car);
            list = list->cdr.value.pair;
        }
    }
    printf(")");
}

void printObject(Object *object)
{
    switch (object->kind)
    {
    case PAIR:
        if (isPair(object))
        {
            printPair(object->value.pair);
        }
        else
        {
            printList(object->value.pair);
        }
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

Object print(Environment *env, Pair *args)
{
    Object car = evaluate(env, args->car);
    printObject(&car);

    args = args->cdr.value.pair;
    while (args != NULL)
    {
        printf(" ");
        car = evaluate(env, args->car);
        printObject(&car);
        args = args->cdr.value.pair;
    }
    printf("\n");
    return booleanObject(true);
}

Object cons(Environment *env, Pair *args)
{
    Pair *pair = malloc(sizeof(Pair));
    pair->car = evaluate(env, args->car);
    pair->cdr = evaluate(env, args->cdr.value.pair->car);
    return pairObject(pair);
}

Object define(Environment *env, Pair *args)
{
    if (args->car.kind != IDENTIFIER)
    {
        printf("Error. Expected identifier.\n");
        exit(-1);
    }

    // TODO: Check empty identifier value
    // if (args.cdr == NULL)
    // {
    //     printf("Error. Expected identifier value.\n");
    //     exit(-1);
    // }

    // Check if cdr is empty
    if (args->cdr.kind == PAIR /* && args->cdr.value.pair->cdr != NULL */)
    {
        printf("Error. Expected exactly 2 arguments.\n");
        exit(-1);
    }

    char *identifier = args->car.value.identifier;
    defineVariable(env, identifier, &args->cdr);
    return booleanObject(true);
}

Object quote(Pair *pair)
{
    return pair->car;
}

// TODO: Add enclosing environments to environments
Object lambda(Pair *args)
{
    // TODO: Check if args are all symbols
    if (!isList(&args->car))
    {
        printf("Error. Args should be a list.\n");
        exit(-1);
    }

    // TODO: Check if params are only args and body
    if (!isList(&args->cdr))
    {
        printf("Error. Body should be a list.\n");
        exit(-1);
    }

    Procedure procedure;
    procedure.parameters = args->car.value.pair;
    procedure.body = args->cdr.value.pair;
    return procedureObject(procedure);
}

Object add(Environment *env, Pair *args)
{
    double result = 0;
    while (args != NULL)
    {
        Object value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result += number;
        args = args->cdr.value.pair;
    }
    return numberObject(result);
}

Object subtract(Environment *env, Pair *args)
{
    Object eval = evaluate(env, args->car);
    double result = getNumber(env, &eval);
    args = args->cdr.value.pair;
    while (args != NULL)
    {
        Object value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result -= number;
        args = args->cdr.value.pair;
    }
    return numberObject(result);
}

Object multiply(Environment *env, Pair *args)
{
    double result = 1;
    while (args != NULL)
    {
        Object value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result *= number;
        args = args->cdr.value.pair;
    }
    return numberObject(result);
}

Object divide(Environment *env, Pair *args)
{
    Object eval = evaluate(env, args->car);
    double result = getNumber(env, &eval);
    args = args->cdr.value.pair;
    while (args != NULL)
    {
        Object value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result /= number;
        args = args->cdr.value.pair;
    }
    return numberObject(result);
}

Object negation(Environment *env, Pair *args)
{
    // check empty list
    // if (args->cdr != NULL)
    // {
    //     printf("Error. Function \"not\" expects one argument, but given more.\n");
    //     exit(-1);
    // }

    return booleanObject(!isTruthy(env, &args->car));
}

size_t listLength(Pair *pair)
{
    size_t length = 0;
    while (pair != NULL)
    {
        length++;
        pair = pair->cdr.value.pair;
    }
    return length;
}

// TODO: Add enclosing environment to procedure
Object executeProcedure(Procedure procedure, Pair *args)
{
    size_t paramsLength = listLength(procedure.parameters);
    size_t argsLength = listLength(args);

    if (paramsLength != argsLength)
    {
        printf("Error. Arity difference in procedure. Expected %ld but got %ld.\n", paramsLength, argsLength);
        exit(-1);
    }

    Environment innerEnv;
    Pair *parameters = procedure.parameters;
    while (args != NULL)
    {
        defineVariable(&innerEnv, parameters->car.value.identifier, &args->car);
        parameters = parameters->cdr.value.pair;
        args = args->cdr.value.pair;
    }

    return evaluate(&innerEnv, pairObject(procedure.body));
}

Object evaluatePair(Environment *env, Pair *pair)
{
    if (pair == NULL)
    {
        return pairObject(pair);
    }

    Object car = pair->car;

    // TODO: Research if identifiers should be tokens for performance and cleaner evaluation
    // Check for default procedures
    if (car.kind == IDENTIFIER)
    {
        char *identifier = car.value.identifier;

        Pair *args = pair->cdr.value.pair;
        if (strcmp(identifier, "cons") == 0)
            return cons(env, args);
        if (strcmp(identifier, "define") == 0)
            return define(env, args);
        if (strcmp(identifier, "lambda") == 0)
            return lambda(args);
        if (strcmp(identifier, "print") == 0)
            return print(env, args);
        if (strcmp(identifier, "quote") == 0)
            return quote(args);

        if (strcmp(identifier, "+") == 0)
            return add(env, args);
        if (strcmp(identifier, "-") == 0)
            return subtract(env, args);
        if (strcmp(identifier, "*") == 0)
            return multiply(env, args);
        if (strcmp(identifier, "/") == 0)
            return divide(env, args);

        if (strcmp(identifier, "not") == 0)
            return negation(env, args);
    }

    car = evaluate(env, pair->car);
    if (car.kind == PROCEDURE)
    {
        Procedure procedure = car.value.procedure;
        return executeProcedure(procedure, pair->cdr.value.pair);
    }

    printf("Error. Could not call value\n");
    exit(-1);
}

Object evaluate(Environment *env, Object object)
{
    if (object.kind == PAIR)
    {
        Pair *pair = object.value.pair;
        return evaluatePair(env, pair);
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