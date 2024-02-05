#include "std.h"

double getNumber(struct Environment *env, struct Object *object)
{
    struct Object eval = evaluate(env, *object);
    if (!isNumber(&eval))
    {
        printf("Error. Expression should eval to number type.\n");
        exit(-1);
    }
    return eval.value.number;
}

struct Object add(struct Environment *env, struct Pair *args)
{
    double result = 0;
    while (args != NULL)
    {
        struct Object value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result += number;
        args = args->cdr.value.pair;
    }
    return numberObject(result);
}

struct Object subtract(struct Environment *env, struct Pair *args)
{
    struct Object eval = evaluate(env, args->car);
    double result = getNumber(env, &eval);
    args = args->cdr.value.pair;
    while (args != NULL)
    {
        struct Object value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result -= number;
        args = args->cdr.value.pair;
    }
    return numberObject(result);
}

struct Object multiply(struct Environment *env, struct Pair *args)
{
    double result = 1;
    while (args != NULL)
    {
        struct Object value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result *= number;
        args = args->cdr.value.pair;
    }
    return numberObject(result);
}

struct Object divide(struct Environment *env, struct Pair *args)
{
    struct Object eval = evaluate(env, args->car);
    double result = getNumber(env, &eval);
    args = args->cdr.value.pair;
    while (args != NULL)
    {
        struct Object value = evaluate(env, args->car);
        double number = getNumber(env, &value);
        result /= number;
        args = args->cdr.value.pair;
    }
    return numberObject(result);
}
