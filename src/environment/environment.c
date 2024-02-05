#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dataTypes.h"

size_t hash(char *key)
{
    size_t hashedKey = 0;

    int i = 1;
    while (key[0] != '\0')
    {
        hashedKey = (hashedKey + key[0] * i) % HASH_SIZE;
        i++;
        key++;
    }
    return hashedKey;
}

struct Object *getVariable(struct Environment *env, char *key)
{
    size_t index = hash(key);

    struct VariableNode *node = env->nodes[index];
    while (node != NULL)
    {
        if (strcmp(key, node->key) == 0)
        {
            return &node->value;
        }
        node = node->next;
    }

    if (env->enclosingEnvironment != NULL)
    {
        return getVariable(env->enclosingEnvironment, key);
    }

    return NULL;
}

void defineVariable(struct Environment *env, char *key, struct Object value)
{
    size_t index = hash(key);
    struct VariableNode *node = env->nodes[index];
    struct VariableNode *previous = NULL;

    while (node != NULL && strcmp(key, node->key) != 0)
    {
        previous = node;
        node = node->next;
    }

    // TODO: Free previous object
    // if (strcmp(key, node->key) == 0)
    // {
    // }

    struct VariableNode *newNode = malloc(sizeof(struct VariableNode));
    newNode->key = key;
    newNode->value = value;
    newNode->next = NULL;

    if (previous == NULL)
    {
        env->nodes[index] = newNode;
    }
    else
    {
        previous->next = newNode;
    }
}

struct Object primitiveProcedureObject(struct Object (*primitiveProcedure)(struct Environment *env, struct Pair *args))
{
    struct Object object;
    object.kind = PRIMITIVE_PROCEDURE;
    object.value.primitiveProcedure = (void *)primitiveProcedure;
    return object;
}
