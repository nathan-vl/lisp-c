#include "std.h"
#include "utils.h"

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
    struct Expression expression = evaluate(env, args->cdr->car);
    if (getVariable(env, symbol) != NULL)
    {
        printf("Error. Variable is already defined.\n");
        exit(-1);
    }
    defineVariable(env, symbol, expression);
    return booleanExpression(true);
}

struct Expression defmacro(struct Environment *env, struct List *args)
{
    checkArityError(3, listLength(args));

    if (args->car.kind != SYMBOL)
    {
        printf("Error. Expected macro name.\n");
        exit(-1);
    }

    char *macroName = args->car.value.symbol;

    if (args->cdr->car.kind != LIST)
    {
        printf("Error. Expected parameters list.\n");
        exit(-1);
    }

    struct List *parametersList = args->cdr->car.value.list;
    size_t parametersLength = listLength(parametersList);

    char **parameters = NULL;
    if (parametersLength > 0)
    {
        parameters = malloc(sizeof(char *) * parametersLength);
        for (size_t i = 0; i < parametersLength; i++)
        {
            if (parametersList->car.kind != SYMBOL)
            {
                printf("Error. Expected symbol in parameter list.\n");
                exit(-1);
            }
            parameters[i] = parametersList->car.value.symbol;
            parametersList = parametersList->cdr;
        }
    }

    if (args->cdr->cdr->car.kind != LIST)
    {
        printf("Error. Expected list in macro body.\n");
        exit(-1);
    }

    struct Macro macro;
    macro.parameters = parameters;
    macro.parametersLength = parametersLength;
    macro.body = args->cdr->cdr->car;

    defineVariable(env, macroName, macroExpression(macro));

    return booleanExpression(true);
}

struct Expression ifExpr(struct Environment *env, struct List *args)
{
    checkArityError(3, listLength(args));

    if (isTruthy(evaluate(env, args->car)))
    {
        return evaluate(env, args->cdr->car);
    }

    return evaluate(env, args->cdr->cdr->car);
}

struct Expression include(struct Environment *env, struct List *args)
{
    checkArityError(1, listLength(args));

    if (args->car.kind != STRING)
    {
        printf("Error. Expected string for file path.\n");
        exit(-1);
    }

    char *path = args->car.value.string;

    includeFile(env, path);

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
            if (car.kind != SYMBOL)
            {
                printf("Error. Expected symbol.\n");
            }
            procedure.parameters[i] = currentParam->car.value.symbol;
            currentParam = currentParam->cdr;
        }
    }

    procedure.body = malloc(sizeof(struct Expression));
    *procedure.body = args->cdr->car;
    return procedureExpression(procedure);
}

struct Expression macroExpand(struct Environment *env, struct List *args)
{
    checkArityError(1, listLength(args));

    struct Expression macroCall = evaluate(env, args->car);

    if (macroCall.kind != LIST)
    {
        printf("Error. Expected list for argument.\n");
        exit(-1);
    }

    struct List *macroCallList = macroCall.value.list;

    if (macroCallList == NULL)
    {
        printf("Error. List to macro expand is empty.\n");
        exit(-1);
    }

    struct Expression macroExpr = evaluate(env, macroCallList->car);

    if (macroExpr.kind != MACRO)
    {
        printf("Error. Expected macro to be expanded in list.\n");
        exit(-1);
    }

    struct Macro macro = *macroExpr.value.macro;
    struct List *macroArgs = macroCallList->cdr;

    return replaceMacro(env, macro, macroArgs);
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

struct Expression progn(struct Environment *env, struct List *args)
{
    checkArityAtLeastError(1, listLength(args));

    while (true)
    {
        struct Expression expr = evaluate(env, args->car);
        if (args->cdr == NULL)
        {
            return expr;
        }
        args = args->cdr;
    }
}

struct Expression quote(struct Environment *env, struct List *list)
{
    return env == NULL ? list->car : list->car;
}

struct Expression setValue(struct Environment *env, struct List *args)
{
    checkArityError(2, listLength(args));

    if (args->car.kind != SYMBOL)
    {
        printf("Error. Expected identifier.\n");
        exit(-1);
    }

    char *symbol = args->car.value.symbol;
    defineVariable(env, symbol, evaluate(env, args->cdr->car));

    return booleanExpression(true);
}

struct Expression whileExpr(struct Environment *env, struct List *args)
{
    checkArityError(2, listLength(args));

    if (args->cdr->car.kind != LIST)
    {
        printf("Error. Expected list of expressions.\n");
        exit(-1);
    }

    while (isTruthy(evaluate(env, args->car)))
    {
        struct List *body = args->cdr->car.value.list;
        while (body != NULL)
        {
            evaluate(env, body->car);
            body = body->cdr;
        }
    }

    return booleanExpression(true);
}
