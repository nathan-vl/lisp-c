#pragma once

#include "expression.h"

#define HASH_SIZE 20

struct Environment
{
    struct Environment *enclosingEnvironment;
    struct VariableNode *nodes[HASH_SIZE];
};

struct VariableNode
{
    char *key;
    struct Expression value;

    struct VariableNode *next;
};

struct Expression *getVariable(struct Environment *env, char *key);
void defineVariable(struct Environment *env, char *key, struct Expression value);
struct Expression primitiveProcedureExpression(struct Expression (*primitiveProcedure)(struct Environment *env, struct List *args));
void loadPrimitiveProcedures(struct Environment *env);
