#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "object.h"
#include "environment.h"

struct Object evaluate(struct Environment *env, struct Object object);

void checkArityError(size_t expected, size_t actual)
{
    if (expected != actual)
    {
        printf("Error. Arity difference in procedure. Expected %ld but got %ld.\n", expected, actual);
        exit(-1);
    }
}

struct Object executeProcedure(struct Environment *env, struct Procedure procedure, struct Pair *args)
{
    size_t argsLength = listLength(args);

    checkArityError(procedure.parametersLength, argsLength);

    struct Environment innerEnv;
    innerEnv.enclosingEnvironment = env;

    struct Pair *current = args;
    for (size_t i = 0; i < procedure.parametersLength; i++)
    {
        struct Object object = current->car;
        defineVariable(&innerEnv, procedure.parameters[i], object);
        current = current->cdr.value.pair;
    }

    return evaluate(&innerEnv, pairObject(procedure.body));
}

struct Object evaluatePair(struct Environment *env, struct Pair *pair)
{
    if (pair == NULL)
    {
        return pairObject(pair);
    }

    struct Object object = evaluate(env, pair->car);
    switch (object.kind)
    {
    case PRIMITIVE_PROCEDURE:
        struct Object (*primitiveProcedure)(struct Environment *env, struct Pair *args) = object.value.primitiveProcedure;
        return primitiveProcedure(env, pair->cdr.value.pair);
    case PROCEDURE:
        struct Procedure procedure = object.value.procedure;
        return executeProcedure(env, procedure, pair->cdr.value.pair);
    default:
        printf("Error. Could not call value\n");
        exit(-1);
    }
}

struct Object evaluateIdentifier(struct Environment *env, char *identifier)
{
    struct Object *var = getVariable(env, identifier);
    if (var == NULL)
    {
        printf("Error. Variable \"%s\" is not defined.\n", identifier);
        exit(-1);
    }
    return evaluate(env, *var);
}

struct Object evaluate(struct Environment *env, struct Object object)
{
    switch (object.kind)
    {
    case PAIR:
        struct Pair *pair = object.value.pair;
        return evaluatePair(env, pair);
    case IDENTIFIER:
        char *identifier = object.value.identifier;
        return evaluateIdentifier(env, identifier);
    default:
        return object;
    }
}
