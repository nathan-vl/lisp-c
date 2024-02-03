#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "environment.h"
#include "eval.h"

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

    Environment env;
    env.enclosingEnvironment = NULL;

    for (;;)
    {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin))
        {
            printf("\n");
            break;
        }

        TokenLinkedList *tokens = parse(line);
        ObjectLinkedList *objects = syntaxAnalyser(tokens);
        while (tokens != NULL)
        {
            TokenLinkedList *current = tokens;
            tokens = tokens->next;
            free(current);
        }

        while (objects != NULL)
        {
            Object objectEval = evaluate(&env, objects->value);
            printObject(&objectEval);
            printf("\n");

            objects = objects->next;
        }
    }
}

void freeObject(Object *object)
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

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        interpreter();
    }
    else if (argc == 2)
    {
        char *fileContents = readFile(argv[1]);

        TokenLinkedList *tokens = parse(fileContents);
        free(fileContents);

        ObjectLinkedList *objects = syntaxAnalyser(tokens);
        while (tokens != NULL)
        {
            TokenLinkedList *current = tokens;
            tokens = tokens->next;
            free(current);
        }

        Environment env;
        env.enclosingEnvironment = NULL;
        while (objects != NULL)
        {
            evaluate(&env, objects->value);
            ObjectLinkedList *current = objects;

            objects = objects->next;

            freeObject(&current->value);
            free(current);
        }

        return -1;
    }

    return 0;
}
