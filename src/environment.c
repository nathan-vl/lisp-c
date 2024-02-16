#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "environment.h"
#include "std.h"
#include "utils.h"

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

struct Environment createEnvironment(struct Environment *enclosingEnvironment)
{
    struct Environment env;
    env.enclosingEnvironment = enclosingEnvironment;
    for (size_t i = 0; i < HASH_SIZE; i++)
    {
        env.nodes[i] = NULL;
    }

    return env;
}

struct Environment bootstrapEnvironment()
{
    struct Environment env = createBaseEnvironment();

    includeFile(&env, "src/std/core.nn");

    return env;
}

void loadPrimitiveProcedures(struct Environment *env)
{
    defineVariable(env, "cons", primitiveProcedureExpression(cons));
    defineVariable(env, "define", primitiveProcedureExpression(define));
    defineVariable(env, "defmacro", primitiveProcedureExpression(defmacro));
    defineVariable(env, "if", primitiveProcedureExpression(ifExpr));
    defineVariable(env, "include", primitiveProcedureExpression(include));
    defineVariable(env, "lambda", primitiveProcedureExpression(lambda));
    defineVariable(env, "macroexpand", primitiveProcedureExpression(macroExpand));
    defineVariable(env, "print", primitiveProcedureExpression(print));
    defineVariable(env, "progn", primitiveProcedureExpression(progn));
    defineVariable(env, "quote", primitiveProcedureExpression(quote));
    defineVariable(env, "set!", primitiveProcedureExpression(setValue));
    defineVariable(env, "while", primitiveProcedureExpression(whileExpr));

    defineVariable(env, "=", primitiveProcedureExpression(equals));
    defineVariable(env, "<", primitiveProcedureExpression(lessThan));
    defineVariable(env, ">", primitiveProcedureExpression(greaterThan));
    defineVariable(env, "<=", primitiveProcedureExpression(lessEqualThan));
    defineVariable(env, ">=", primitiveProcedureExpression(greaterEqualThan));
    defineVariable(env, "not", primitiveProcedureExpression(negation));
    defineVariable(env, "and", primitiveProcedureExpression(andExpr));
    defineVariable(env, "or", primitiveProcedureExpression(orExpr));

    defineVariable(env, "+", primitiveProcedureExpression(add));
    defineVariable(env, "-", primitiveProcedureExpression(subtract));
    defineVariable(env, "*", primitiveProcedureExpression(multiply));
    defineVariable(env, "/", primitiveProcedureExpression(divide));
    defineVariable(env, "pow", primitiveProcedureExpression(exponentiation));
    defineVariable(env, "%", primitiveProcedureExpression(modulo));
}

struct Environment createBaseEnvironment()
{
    struct Environment env = createEnvironment(NULL);

    loadPrimitiveProcedures(&env);

    return env;
}

struct Expression *getVariable(struct Environment *env, char *key)
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

void defineVariable(struct Environment *env, char *key, struct Expression value)
{
    size_t index = hash(key);
    struct VariableNode *node = env->nodes[index];
    struct VariableNode *previous = NULL;

    while (node != NULL && strcmp(key, node->key) != 0)
    {
        previous = node;
        node = node->next;
    }

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

struct Expression primitiveProcedureExpression(struct Expression (*primitiveProcedure)(struct Environment *env, struct List *args))
{
    struct Expression expression;
    expression.kind = PRIMITIVE_PROCEDURE;
    expression.value.primitiveProcedure = (void *)primitiveProcedure;
    return expression;
}
