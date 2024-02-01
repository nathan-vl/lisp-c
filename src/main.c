#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "environment.h"
#include "eval.h"
#include "object.h"

void printObject(Object *object)
{
    switch (object->kind)
    {
    case LIST:
        List *list = object->value.list;
        printf("(");
        if (list != NULL)
        {
            printObject(&list->car);

            list = list->cdr;
            while (list != NULL)
            {
                printf(" ");
                printObject(&list->car);
                list = list->cdr;
            }
        }
        printf(")");
        break;
    case BOOLEAN:
        printf("%s", object->value.boolean ? "#t" : "#f");
        break;
    case CHARACTER:
        printf("%c", object->value.character);
        break;
    case IDENTIFIER:
        printf("%s", object->value.identifier);
        break;
    case NUMBER:
        printf("%f", object->value.number);
        break;
    case STRING:
        printf("\"%s\"", object->value.string);
        break;
    }
}

void interpreter()
{
    char line[1024];

    Environment env;

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

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        interpreter();
    }
    else if (argc == 2)
    {
        printf("Error. File reader not implemented.\n");
        return -1;
    }

    return 0;
}
