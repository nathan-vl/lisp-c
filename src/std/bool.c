#include "std.h"

struct Object negation(struct Environment *env, struct Pair *args)
{
    checkArityError(1, listLength(args));

    struct Object object = evaluate(env, args->car);
    return booleanObject(!isTruthy(&object));
}
