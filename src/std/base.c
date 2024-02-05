#include "std.h"

struct Object cons(struct Environment *env, struct Pair *args)
{
    checkArityError(2, listLength(args));

    struct Pair *pair = malloc(sizeof(struct Pair));
    pair->car = evaluate(env, args->car);
    pair->cdr = evaluate(env, args->cdr.value.pair->car);
    return pairObject(pair);
}

struct Object define(struct Environment *env, struct Pair *args)
{
    if (args->car.kind != IDENTIFIER)
    {
        printf("Error. Expected identifier.\n");
        exit(-1);
    }

    checkArityError(2, listLength(args));

    char *identifier = args->car.value.identifier;
    struct Object object = args->cdr.value.pair->car;
    defineVariable(env, identifier, object);
    return booleanObject(true);
}

struct Object lambda(struct Environment *env, struct Pair *args)
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
            struct Object car = currentParam->car;
            if (car.kind != IDENTIFIER)
            {
                printf("Error. Expected identifier.\n");
            }
            procedure.parameters[i] = currentParam->car.value.identifier;
            currentParam = currentParam->cdr.value.pair;
        }
    }

    struct Object body = args->cdr.value.pair->car;
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
    return procedureObject(procedure);
}

struct Object print(struct Environment *env, struct Pair *args)
{
    struct Object car = evaluate(env, args->car);
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

struct Object quote(struct Environment *env, struct Pair *pair)
{
    return env == NULL ? pair->car : pair->car;
}
