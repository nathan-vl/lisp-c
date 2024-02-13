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
    checkArityError(2, listLength(args));

    if (args->car.kind != SYMBOL)
    {
        printf("Error. Expected symbol.\n");
        exit(-1);
    }

    char *symbol = args->car.value.symbol;
    struct Expression expression = args->cdr->car;
    defineVariable(env, symbol, expression);
    return booleanExpression(true);
}

struct Expression ifExpr(struct Environment *env, struct List *args)
{
    checkArityError(3, listLength(args));

    struct Expression cond = evaluate(env, args->car);

    if (isTruthy(&cond))
    {
        return evaluate(env, args->cdr->car);
    }

    return evaluate(env, args->cdr->cdr->car);
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
            if (car.kind != SYMBOL)
            {
                printf("Error. Expected symbol.\n");
            }
            procedure.parameters[i] = currentParam->car.value.symbol;
            currentParam = currentParam->cdr;
        }
    }

    struct Expression body = args->cdr->car;
    if (body.kind == SYMBOL)
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