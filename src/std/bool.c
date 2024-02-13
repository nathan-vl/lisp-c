#include "std.h"

#include <string.h>

struct Expression negation(struct Environment *env, struct List *args)
{
    checkArityError(1, listLength(args));

    struct Expression expression = evaluate(env, args->car);
    return booleanExpression(!isTruthy(&expression));
}

struct Expression equals(struct Environment *env, struct List *args)
{
    checkArityError(2, listLength(args));

    struct Expression firstExpr = evaluate(env, args->car);
    struct Expression secondExpr = evaluate(env, args->cdr->car);

    if (firstExpr.kind != secondExpr.kind)
    {
        return booleanExpression(false);
    }

    switch (firstExpr.kind)
    {
    case BOOLEAN:
        return booleanExpression(firstExpr.value.boolean == secondExpr.value.boolean);
    case CHARACTER:
        return booleanExpression(firstExpr.value.character == secondExpr.value.character);
    case SYMBOL:
        return booleanExpression(strcmp(firstExpr.value.symbol, secondExpr.value.symbol) == 0);
    case NUMBER:
        return booleanExpression(firstExpr.value.number == secondExpr.value.number);
    case PRIMITIVE_PROCEDURE:
        return booleanExpression(strcmp(args->car.value.symbol, args->cdr->car.value.symbol) == 0);
    case STRING:
        return booleanExpression(strcmp(firstExpr.value.string, secondExpr.value.string) == 0);
    case LIST:
    case PROCEDURE:
    default:
        return booleanExpression(false);
    }
}
