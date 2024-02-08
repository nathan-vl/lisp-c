#include "std.h"

struct Expression negation(struct Environment *env, struct List *args)
{
    checkArityError(1, listLength(args));

    struct Expression expression = evaluate(env, args->car);
    return booleanExpression(!isTruthy(&expression));
}
