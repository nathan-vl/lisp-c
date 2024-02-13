#include <stdio.h>
#include <stdlib.h>

#include "eval.h"
#include "std.h"

void loadPrimitiveProcedures(struct Environment *env)
{
    defineVariable(env, "cons", primitiveProcedureExpression(cons));
    defineVariable(env, "define", primitiveProcedureExpression(define));
    defineVariable(env, "=", primitiveProcedureExpression(equals));
    defineVariable(env, "if", primitiveProcedureExpression(ifExpr));
    defineVariable(env, "lambda", primitiveProcedureExpression(lambda));
    defineVariable(env, "print", primitiveProcedureExpression(print));
    defineVariable(env, "quote", primitiveProcedureExpression(quote));

    defineVariable(env, "+", primitiveProcedureExpression(add));
    defineVariable(env, "-", primitiveProcedureExpression(subtract));
    defineVariable(env, "*", primitiveProcedureExpression(multiply));
    defineVariable(env, "/", primitiveProcedureExpression(divide));

    defineVariable(env, "not", primitiveProcedureExpression(negation));
}

char *readFile(char *path)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL)
    {
        printf("Error. Could not open file \"%s\".\n", path);
    }

    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(size + 1);
    if (buffer == NULL)
    {
        printf("Error. Not enough memory to read file.");
    }

    size_t bytesRead = fread(buffer, sizeof(char), size, file);
    if (bytesRead < size)
    {
        printf("Error. Could not read file \"%s\".\n", path);
    }
    buffer[size - 1] = '\0';

    fclose(file);

    return buffer;
}

void interpreter()
{
    char line[1024];

    struct Environment env;
    env.enclosingEnvironment = NULL;

    loadPrimitiveProcedures(&env);

    for (;;)
    {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin))
        {
            printf("\n");
            break;
        }

        struct TokenLinkedList *tokens = parse(line);
        struct ExpressionLinkedList *expressions = syntaxAnalyser(tokens);
        while (tokens != NULL)
        {
            struct TokenLinkedList *current = tokens;
            tokens = tokens->next;
            free(current);
        }

        while (expressions != NULL)
        {
            struct Expression expressionEval = evaluate(&env, expressions->value);
            printExpression(&expressionEval);
            printf("\n");

            expressions = expressions->next;
        }
    }
}

void freeExpression(struct Expression *expression)
{
    switch (expression->kind)
    {
    case STRING:
        free(expression->value.string);
        break;
    case SYMBOL:
        free(expression->value.symbol);
        break;
    default:
        break;
    }
}

void parseFile(char *path)
{
    char *fileContents = readFile(path);

    struct TokenLinkedList *tokens = parse(fileContents);
    free(fileContents);

    struct ExpressionLinkedList *expressions = syntaxAnalyser(tokens);
    while (tokens != NULL)
    {
        struct TokenLinkedList *current = tokens;
        tokens = tokens->next;
        free(current);
    }

    struct Environment env;
    env.enclosingEnvironment = NULL;

    loadPrimitiveProcedures(&env);

    while (expressions != NULL)
    {
        evaluate(&env, expressions->value);
        struct ExpressionLinkedList *current = expressions;

        expressions = expressions->next;

        freeExpression(&current->value);
        free(current);
    }
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        interpreter();
        return 0;
    }

    if (argc == 2)
    {
        parseFile(argv[1]);
        return 0;
    }

    return 0;
}
