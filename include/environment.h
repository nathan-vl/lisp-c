#pragma once

#include "object.h"

#define HASH_SIZE 20

struct Environment
{
    struct Environment *enclosingEnvironment;
    struct VariableNode *nodes[HASH_SIZE];
};

struct VariableNode
{
    char *key;
    struct Object value;

    struct VariableNode *next;
};

struct Object *getVariable(struct Environment *env, char *key);
void defineVariable(struct Environment *env, char *key, struct Object value);
struct Object primitiveProcedureObject(struct Object (*primitiveProcedure)(struct Environment *env, struct Pair *args));
void loadPrimitiveProcedures(struct Environment *env);
