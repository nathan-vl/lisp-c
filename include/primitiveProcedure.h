#ifndef PRIMITIVE_PROCEDURE_H
#define PRIMITIVE_PROCEDURE_H

#include <stdbool.h>
#include "object.h"

Object primitiveProcedureObject(Object (*primitiveProcedure)(struct Environment *env, struct Pair *args));
void loadPrimitiveProcedures(Environment *env);

#endif
