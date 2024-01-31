#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "object.h"

#define HASH_SIZE 20

typedef struct VariableNode
{
    char *key;
    Object *value;

    struct VariableNode *next;
} VariableNode;

typedef struct Environment
{
    VariableNode *nodes[HASH_SIZE];
} Environment;

Object *getVariable(Environment *env, char *key);
void defineVariable(Environment *env, char *key, Object *value);

#endif
