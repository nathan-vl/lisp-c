#include "std.h"

struct Expression cons(struct Environment *env, struct List *args)
{
    checkArityError(2, listLength(args));

    struct List *list = malloc(sizeof(struct List));
    list->car = evaluate(env, args->car);
    list->cdr = malloc(sizeof(struct List));
    list->cdr->car = evaluate(env, args->cdr->car);
    list->cdr->cdr = NULL;

    return listExpression(list);
}

struct Expression define(struct Environment *env, struct List *args)
{
    if (args->car.kind != IDENTIFIER)
    {
        printf("Error. Expected identifier.\n");
        exit(-1);
    }

    checkArityError(2, listLength(args));

    char *identifier = args->car.value.identifier;
    struct Expression expression = args->cdr->car;
    defineVariable(env, identifier, expression);
    return booleanExpression(true);
}

struct Expression lambda(struct Environment *env, struct List *args)
{
    checkArityError(2, listLength(args));

    if (!isList(&args->car))
    {
        printf("Error. Args should be a list.\n");
        exit(-1);
    }

    struct Procedure procedure;
    procedure.parametersLength = listLength(args->car.value.list);
    if (procedure.parametersLength == 0)
    {
        procedure.parameters = NULL;
    }
    else
    {
        procedure.parameters = malloc(sizeof(char *) * procedure.parametersLength);
        struct List *currentParam = args->car.value.list;
        for (size_t i = 0; i < procedure.parametersLength; i++)
        {
            struct Expression car = currentParam->car;
            if (car.kind != IDENTIFIER)
            {
                printf("Error. Expected identifier.\n");
            }
            procedure.parameters[i] = currentParam->car.value.identifier;
            currentParam = currentParam->cdr;
        }
    }

    struct Expression body = args->cdr->car;
    if (body.kind == IDENTIFIER)
    {
        body = evaluate(env, body);
    }

    if (body.kind != LIST)
    {
        printf("Error. Body should be a list.\n");
        exit(-1);
    }

    procedure.body = body.value.list;
    return procedureExpression(procedure);
}

struct Expression print(struct Environment *env, struct List *args)
{
    struct Expression car = evaluate(env, args->car);
    printExpression(&car);

    args = args->cdr;
    while (args != NULL)
    {
        printf(" ");
        car = evaluate(env, args->car);
        printExpression(&car);
        args = args->cdr;
    }
    printf("\n");
    return booleanExpression(true);
}

struct Expression quote(struct Environment *env, struct List *list)
{
    return env == NULL ? list->car : list->car;
}
