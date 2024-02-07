#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression.h"
#include "environment.h"

struct Expression evaluate(struct Environment *env, struct Expression expression);

void checkArityError(size_t expected, size_t actual)
{
    if (expected != actual)
    {
        printf("Error. Arity difference in procedure. Expected %ld but got %ld.\n", expected, actual);
        exit(-1);
    }
}

struct Expression executeProcedure(struct Environment *env, struct Procedure procedure, struct Pair *args)
{
    size_t argsLength = listLength(args);

    checkArityError(procedure.parametersLength, argsLength);

    struct Environment innerEnv;
    innerEnv.enclosingEnvironment = env;

    struct Pair *current = args;
    for (size_t i = 0; i < procedure.parametersLength; i++)
    {
        struct Expression expression = current->car;
        defineVariable(&innerEnv, procedure.parameters[i], expression);
        current = current->cdr.value.pair;
    }

    return evaluate(&innerEnv, pairExpression(procedure.body));
}

struct Expression evaluatePair(struct Environment *env, struct Pair *pair)
{
    if (pair == NULL)
    {
        return pairExpression(pair);
    }

    struct Expression expression = evaluate(env, pair->car);
    switch (expression.kind)
    {
    case PRIMITIVE_PROCEDURE:
        struct Expression (*primitiveProcedure)(struct Environment *env, struct Pair *args) = expression.value.primitiveProcedure;
        return primitiveProcedure(env, pair->cdr.value.pair);
    case PROCEDURE:
        struct Procedure procedure = expression.value.procedure;
        return executeProcedure(env, procedure, pair->cdr.value.pair);
    default:
        printf("Error. Could not call value\n");
        exit(-1);
    }
}

struct Expression evaluateIdentifier(struct Environment *env, char *identifier)
{
    struct Expression *var = getVariable(env, identifier);
    if (var == NULL)
    {
        printf("Error. Variable \"%s\" is not defined.\n", identifier);
        exit(-1);
    }
    return evaluate(env, *var);
}

struct Expression evaluate(struct Environment *env, struct Expression expression)
{
    switch (expression.kind)
    {
    case PAIR:
        struct Pair *pair = expression.value.pair;
        return evaluatePair(env, pair);
    case IDENTIFIER:
        char *identifier = expression.value.identifier;
        return evaluateIdentifier(env, identifier);
    default:
        return expression;
    }
}
