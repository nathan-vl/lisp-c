#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "object.h"
#include "environment/dataTypes.h"

struct Object *getVariable(struct Environment *env, char *key);
void defineVariable(struct Environment *env, char *key, struct Object value);
struct Object primitiveProcedureObject(struct Object (*primitiveProcedure)(struct Environment *env, struct Pair *args));
void loadPrimitiveProcedures(struct Environment *env);

#endif
