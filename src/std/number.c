#include "std.h"

double getNumber(struct Environment *env, struct Expression *expression)
{
    struct Expression eval = evaluate(env, *expression);
    if (!isNumber(&eval))
    {
        printf("Error. Expression should eval to number type.\n");
        exit(-1);
    }
    return eval.value.number;
}

struct Expression add(struct Environment *env, struct Pair *args)
{
    double result = 0;
    while (args != NULL)
    {
        struct Expression value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result += number;
        args = args->cdr.value.pair;
    }
    return numberExpression(result);
}

struct Expression subtract(struct Environment *env, struct Pair *args)
{
    checkArityAtLeastError(1, listLength(args));

    struct Expression eval = evaluate(env, args->car);
    double result = getNumber(env, &eval);
    args = args->cdr.value.pair;
    if (args == NULL)
    {
        return numberExpression(-result);
    }

    while (args != NULL)
    {
        struct Expression value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result -= number;
        args = args->cdr.value.pair;
    }
    return numberExpression(result);
}

struct Expression multiply(struct Environment *env, struct Pair *args)
{
    double result = 1;
    while (args != NULL)
    {
        struct Expression value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result *= number;
        args = args->cdr.value.pair;
    }
    return numberExpression(result);
}

struct Expression divide(struct Environment *env, struct Pair *args)
{
    struct Expression eval = evaluate(env, args->car);
    double result = getNumber(env, &eval);
    args = args->cdr.value.pair;
    while (args != NULL)
    {
        struct Expression value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result /= number;
        args = args->cdr.value.pair;
    }
    return numberExpression(result);
}
