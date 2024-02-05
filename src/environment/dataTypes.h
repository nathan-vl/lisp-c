#ifndef ENVIRONMENT_DATA_TYPES_H
#define ENVIRONMENT_DATA_TYPES_H

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

#endif
