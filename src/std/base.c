#include "std.h"

struct Expression cons(struct Environment *env, struct Pair *args)
{
    checkArityError(2, listLength(args));

    struct Pair *pair = malloc(sizeof(struct Pair));
    pair->car = evaluate(env, args->car);
    pair->cdr = evaluate(env, args->cdr.value.pair->car);
    return pairExpression(pair);
}

struct Expression define(struct Environment *env, struct Pair *args)
{
    if (args->car.kind != IDENTIFIER)
    {
        printf("Error. Expected identifier.\n");
        exit(-1);
    }

    checkArityError(2, listLength(args));

    char *identifier = args->car.value.identifier;
    struct Expression expression = args->cdr.value.pair->car;
    defineVariable(env, identifier, expression);
    return booleanExpression(true);
}

struct Expression lambda(struct Environment *env, struct Pair *args)
{
    checkArityError(2, listLength(args));

    if (!isList(&args->car))
    {
        printf("Error. Args should be a list.\n");
        exit(-1);
    }

    struct Procedure procedure;
    procedure.parametersLength = listLength(args->car.value.pair);
    if (procedure.parametersLength == 0)
    {
        procedure.parameters = NULL;
    }
    else
    {
        procedure.parameters = malloc(sizeof(char *) * procedure.parametersLength);
        struct Pair *currentParam = args->car.value.pair;
        for (size_t i = 0; i < procedure.parametersLength; i++)
        {
            struct Expression car = currentParam->car;
            if (car.kind != IDENTIFIER)
            {
                printf("Error. Expected identifier.\n");
            }
            procedure.parameters[i] = currentParam->car.value.identifier;
            currentParam = currentParam->cdr.value.pair;
        }
    }

    struct Expression body = args->cdr.value.pair->car;
    if (body.kind == IDENTIFIER)
    {
        body = evaluate(env, body);
    }

    if (body.kind != PAIR)
    {
        printf("Error. Body should be a list.\n");
        exit(-1);
    }

    procedure.body = body.value.pair;
    return procedureExpression(procedure);
}

struct Expression print(struct Environment *env, struct Pair *args)
{
    struct Expression car = evaluate(env, args->car);
    printExpression(&car);

    args = args->cdr.value.pair;
    while (args != NULL)
    {
        printf(" ");
        car = evaluate(env, args->car);
        printExpression(&car);
        args = args->cdr.value.pair;
    }
    printf("\n");
    return booleanExpression(true);
}

struct Expression quote(struct Environment *env, struct Pair *pair)
{
    return env == NULL ? pair->car : pair->car;
}
