#include "std.h"

double getNumber(Environment *env, Object *object)
{
    Object eval = evaluate(env, *object);
    if (!isNumber(&eval))
    {
        printf("Error. Expression should eval to number type.\n");
        exit(-1);
    }
    return eval.value.number;
}

Object add(struct Environment *env, struct Pair *args)
{
    double result = 0;
    while (args != NULL)
    {
        Object value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result += number;
        args = args->cdr.value.pair;
    }
    return numberObject(result);
}

Object subtract(struct Environment *env, struct Pair *args)
{
    Object eval = evaluate(env, args->car);
    double result = getNumber(env, &eval);
    args = args->cdr.value.pair;
    while (args != NULL)
    {
        Object value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result -= number;
        args = args->cdr.value.pair;
    }
    return numberObject(result);
}

Object multiply(struct Environment *env, struct Pair *args)
{
    double result = 1;
    while (args != NULL)
    {
        Object value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result *= number;
        args = args->cdr.value.pair;
    }
    return numberObject(result);
}

Object divide(struct Environment *env, struct Pair *args)
{
    Object eval = evaluate(env, args->car);
    double result = getNumber(env, &eval);
    args = args->cdr.value.pair;
    while (args != NULL)
    {
        Object value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result /= number;
        args = args->cdr.value.pair;
    }
    return numberObject(result);
}
