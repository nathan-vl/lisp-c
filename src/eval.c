#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eval.h"
#include "error.h"
#include "util.h"

Object executeProcedure(Environment *env, Procedure procedure, Pair *args)
{
    size_t argsLength = listLength(args);

    checkArityError(procedure.parametersLength, argsLength);

    Environment innerEnv;
    innerEnv.enclosingEnvironment = env;

    Pair *current = args;
    for (size_t i = 0; i < procedure.parametersLength; i++)
    {
        Object object = current->car;
        defineVariable(&innerEnv, procedure.parameters[i], object);
        current = current->cdr.value.pair;
    }

    return evaluate(&innerEnv, pairObject(procedure.body));
}

Object evaluatePair(struct Environment *env, struct Pair *pair)
{
    if (pair == NULL)
    {
        return pairObject(pair);
    }

    Object object = evaluate(env, pair->car);
    switch (object.kind)
    {
    case PRIMITIVE_PROCEDURE:
        Object (*primitiveProcedure)(struct Environment *env, struct Pair *args) = object.value.primitiveProcedure;
        return primitiveProcedure(env, pair->cdr.value.pair);
    case PROCEDURE:
        Procedure procedure = object.value.procedure;
        return executeProcedure(env, procedure, pair->cdr.value.pair);
    default:
        printf("Error. Could not call value\n");
        exit(-1);
    }
}

Object evaluateIdentifier(Environment *env, char *identifier)
{
    Object *var = getVariable(env, identifier);
    if (var == NULL)
    {
        printf("Error. Variable \"%s\" is not defined.\n", identifier);
        exit(-1);
    }
    return evaluate(env, *var);
}

Object evaluate(Environment *env, Object object)
{
    switch (object.kind)
    {
    case PAIR:
        Pair *pair = object.value.pair;
        return evaluatePair(env, pair);
    case IDENTIFIER:
        char *identifier = object.value.identifier;
        return evaluateIdentifier(env, identifier);
    default:
        return object;
    }
}
