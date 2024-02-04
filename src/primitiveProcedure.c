#include "std.h"

Object primitiveProcedureObject(Object (*primitiveProcedure)(struct Environment *env, struct Pair *args))
{
    Object object;
    object.kind = PRIMITIVE_PROCEDURE;
    object.value.primitiveProcedure = (void *)primitiveProcedure;
    return object;
}

void loadPrimitiveProcedures(Environment *env)
{
    defineVariable(env, "cons", primitiveProcedureObject(cons));
    defineVariable(env, "define", primitiveProcedureObject(define));
    defineVariable(env, "lambda", primitiveProcedureObject(lambda));
    defineVariable(env, "print", primitiveProcedureObject(print));
    defineVariable(env, "quote", primitiveProcedureObject(quote));

    defineVariable(env, "+", primitiveProcedureObject(add));
    defineVariable(env, "-", primitiveProcedureObject(subtract));
    defineVariable(env, "*", primitiveProcedureObject(multiply));
    defineVariable(env, "/", primitiveProcedureObject(divide));

    defineVariable(env, "not", primitiveProcedureObject(negation));
}
