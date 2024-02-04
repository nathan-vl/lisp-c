#include "std.h"

Object negation(struct Environment *env, struct Pair *args)
{
    checkArityError(1, listLength(args));

    Object object = evaluate(env, args->car);
    return booleanObject(!isTruthy(&object));
}
