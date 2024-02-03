#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eval.h"

void checkArityError(size_t expected, size_t actual)
{
    if (expected != actual)
    {
        printf("Error. Arity difference in procedure. Expected %ld but got %ld.\n", expected, actual);
        exit(-1);
    }
}

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

Object define(Environment *env, Pair *args)
{
    if (args->car.kind != IDENTIFIER)
    {
        printf("Error. Expected identifier.\n");
        exit(-1);
    }

    checkArityError(2, listLength(args));

    char *identifier = args->car.value.identifier;
    defineVariable(env, identifier, &args->cdr.value.pair->car);
    return booleanObject(true);
}

Object quote(Pair *pair)
{
    return pair->car;
}

Object lambda(Pair *args)
{
    checkArityError(2, listLength(args));

    if (!isList(&args->car))
    {
        printf("Error. Args should be a list.\n");
        exit(-1);
    }

    if (!isList(&args->cdr))
    {
        printf("Error. Body should be a list.\n");
        exit(-1);
    }

    Procedure procedure;
    procedure.parametersLength = listLength(args->car.value.pair);
    if (procedure.parametersLength == 0)
    {
        procedure.parameters = NULL;
    }
    else
    {
        procedure.parameters = malloc(sizeof(char *) * procedure.parametersLength);
        Pair *currentParam = args->car.value.pair;
        for (size_t i = 0; i < procedure.parametersLength; i++)
        {
            Object car = currentParam->car;
            if (car.kind != IDENTIFIER)
            {
                printf("Error. Expected identifier.\n");
            }
            procedure.parameters[i] = currentParam->car.value.identifier;
            currentParam = currentParam->cdr.value.pair;
        }
    }

    procedure.body = args->cdr.value.pair->car.value.pair;
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
    checkArityError(1, listLength(args));

    return booleanObject(!isTruthy(env, &args->car));
}

Object executeProcedure(Environment *env, Procedure procedure, Pair *args)
{
    size_t argsLength = listLength(args);

    checkArityError(procedure.parametersLength, argsLength);

    Environment innerEnv;
    innerEnv.enclosingEnvironment = env;

    Pair *current = args;
    for (size_t i = 0; i < procedure.parametersLength; i++)
    {
        defineVariable(&innerEnv, procedure.parameters[i], &current->car);
        current = current->cdr.value.pair;
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
        return executeProcedure(env, procedure, pair->cdr.value.pair);
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
