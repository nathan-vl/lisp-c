#include <stdio.h>
#include <stdlib.h>

#include "eval.h"
#include "std.h"

void loadPrimitiveProcedures(struct Environment *env)
{
    defineVariable(env, "cons", primitiveProcedureObject(cons));
    defineVariable(env, "define", primitiveProcedureObject(define));
    defineVariable(env, "lambda", primitiveProcedureObject(lambda));
    defineVariable(env, "print", primitiveProcedureObject(print));
    defineVariable(env, "quote", primitiveProcedureObject(quote));

    defineVariable(env, "+", primitiveProcedureObject(add));
    defineVariable(env, "-", primitiveProcedureObject(subtract));
    defineVariable(env, "*", primitiveProcedureObject(multiply));
    defineVariable(env, "/", primitiveProcedureObject(divide));

    defineVariable(env, "not", primitiveProcedureObject(negation));
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
        struct ObjectLinkedList *objects = syntaxAnalyser(tokens);
        while (tokens != NULL)
        {
            struct TokenLinkedList *current = tokens;
            tokens = tokens->next;
            free(current);
        }

        while (objects != NULL)
        {
            struct Object objectEval = evaluate(&env, objects->value);
            printObject(&objectEval);
            printf("\n");

            objects = objects->next;
        }
    }
}

void freeObject(struct Object *object)
{
    switch (object->kind)
    {
    case STRING:
        free(object->value.string);
        break;
    case IDENTIFIER:
        free(object->value.identifier);
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

    struct ObjectLinkedList *objects = syntaxAnalyser(tokens);
    while (tokens != NULL)
    {
        struct TokenLinkedList *current = tokens;
        tokens = tokens->next;
        free(current);
    }

    struct Environment env;
    env.enclosingEnvironment = NULL;

    loadPrimitiveProcedures(&env);

    while (objects != NULL)
    {
        evaluate(&env, objects->value);
        struct ObjectLinkedList *current = objects;

        objects = objects->next;

        freeObject(&current->value);
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
