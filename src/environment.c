#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "environment.h"

size_t hash(char *key)
{
    size_t hashedKey = 0;

    int i = 1;
    while (key[0] != '\0')
    {
        hashedKey += (key[0] * i) % HASH_SIZE;
        i++;
        key++;
    }
    return hashedKey;
}

Object *getVariable(Environment *env, char *key)
{
    size_t index = hash(key);

    VariableNode *node = env->nodes[index];
    while (node != NULL)
    {
        if (strcmp(key, node->key) == 0)
        {
            return node->value;
        }
        node = node->next;
    }

    if (env->enclosingEnvironment != NULL)
    {
        return getVariable(env->enclosingEnvironment, key);
    }

    return NULL;
}

void defineVariable(Environment *env, char *key, Object *value)
{
    size_t index = hash(key);
    VariableNode *node = env->nodes[index];
    VariableNode *previous = NULL;

    while (node != NULL)
    {
        if (strcmp(key, node->key) == 0)
        {
            printf("Error. Variable \"%s\" is already defined.\n", key);
        }
        previous = node;
        node = node->next;
    }

    VariableNode *newNode = malloc(sizeof(VariableNode));
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
