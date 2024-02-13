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

void checkArityAtLeastError(size_t minimum, size_t actual)
{
    if (actual < minimum)
    {
        printf("Error. Arity difference in procedure. Expected at least %ld but got %ld.\n", minimum, actual);
        exit(-1);
    }
}

struct Expression executeProcedure(struct Environment *env, struct Procedure procedure, struct List *args)
{
    size_t argsLength = listLength(args);

    checkArityError(procedure.parametersLength, argsLength);

    struct Environment innerEnv;
    innerEnv.enclosingEnvironment = env;

    struct List *current = args;
    for (size_t i = 0; i < procedure.parametersLength; i++)
    {
        char *parameter = procedure.parameters[i];
        struct Expression expression = evaluate(env, current->car);
        defineVariable(&innerEnv, parameter, expression);
        current = current->cdr;
    }

    return evaluate(&innerEnv, listExpression(procedure.body));
}

struct Expression evaluateList(struct Environment *env, struct List *list)
{
    if (list == NULL)
    {
        return listExpression(NULL);
    }

    struct Expression expression = evaluate(env, list->car);
    switch (expression.kind)
    {
    case PRIMITIVE_PROCEDURE:
        struct Expression (*primitiveProcedure)(struct Environment *env, struct List *args) = expression.value.primitiveProcedure;
        return primitiveProcedure(env, list->cdr);
    case PROCEDURE:
        struct Procedure procedure = expression.value.procedure;
        return executeProcedure(env, procedure, list->cdr);
    default:
        printf("Error. Could not call value\n");
        exit(-1);
    }
}

struct Expression evaluateSymbol(struct Environment *env, char *symbol)
{
    struct Expression *var = getVariable(env, symbol);
    if (var == NULL)
    {
        printf("Error. \"%s\" is not defined.\n", symbol);
        exit(-1);
    }
    return evaluate(env, *var);
}

struct Expression evaluate(struct Environment *env, struct Expression expression)
{
    switch (expression.kind)
    {
    case LIST:
        struct List *list = expression.value.list;
        return evaluateList(env, list);
    case SYMBOL:
        char *symbol = expression.value.symbol;
        return evaluateSymbol(env, symbol);
    default:
        return expression;
    }
}
