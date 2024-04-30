#include "std.h"
#include <math.h>

double getNumber(struct Environment *env, struct Expression *expression)
{
    struct Expression eval = evaluate(env, *expression);
    if (!isNumber(&eval))
    {
        printf("Error. Expression should evaluate to number type.\n");
        exit(-1);
    }
    return eval.value.number;
}

struct Expression add(struct Environment *env, struct List *args)
{
    double result = 0;
    while (args != NULL)
    {
        struct Expression value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result += number;
        args = args->cdr;
    }
    return numberExpression(result);
}

struct Expression subtract(struct Environment *env, struct List *args)
{
    checkArityAtLeastError(1, listLength(args));

    struct Expression eval = evaluate(env, args->car);
    double result = getNumber(env, &eval);
    args = args->cdr;
    if (args == NULL)
    {
        return numberExpression(-result);
    }

    while (args != NULL)
    {
        struct Expression value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result -= number;
        args = args->cdr;
    }
    return numberExpression(result);
}

struct Expression multiply(struct Environment *env, struct List *args)
{
    double result = 1;
    while (args != NULL)
    {
        struct Expression value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result *= number;
        args = args->cdr;
    }
    return numberExpression(result);
}

struct Expression divide(struct Environment *env, struct List *args)
{
    struct Expression eval = evaluate(env, args->car);
    double result = getNumber(env, &eval);
    args = args->cdr;
    while (args != NULL)
    {
        struct Expression value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result /= number;
        args = args->cdr;
    }
    return numberExpression(result);
}

struct Expression exponentiation(struct Environment *env, struct List *args)
{
    checkArityError(2, listLength(args));

    struct Expression firstExpr = evaluate(env, args->car);
    if (firstExpr.kind != NUMBER)
    {
        printf("Error. Expected number for first argument.\n");
        exit(-1);
    }
    args = args->cdr;

    struct Expression secondExpr = evaluate(env, args->car);
    if (secondExpr.kind != NUMBER)
    {
        printf("Error. Expected number for second argument.\n");
        exit(-1);
    }

    return numberExpression(powl(firstExpr.value.number, secondExpr.value.number));
}

struct Expression modulo(struct Environment *env, struct List *args)
{
    checkArityError(2, listLength(args));

    struct Expression firstExpr = evaluate(env, args->car);
    if (firstExpr.kind != NUMBER)
    {
        printf("Error. Expected number for first argument.\n");
        exit(-1);
    }
    args = args->cdr;

    struct Expression secondExpr = evaluate(env, args->car);
    if (secondExpr.kind != NUMBER)
    {
        printf("Error. Expected number for second argument.\n");
        exit(-1);
    }

    return numberExpression(fmod(firstExpr.value.number, secondExpr.value.number));
}
