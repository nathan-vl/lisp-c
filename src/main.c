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
    for (;;)
    {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin))
        {
            printf("\n");
            break;
        }

        TokenLinkedList *tokens = parse(line);
        Object object = syntaxAnalyser(tokens);

        Environment env;
        Object objectEval = evaluate(&env, object);
        printObject(&objectEval);
        printf("\n");
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
        TokenLinkedList *current = parse("(+ 2 2)");
        Object object = syntaxAnalyser(current);

        printObject(&object);
        printf("\n");
    }

    return 0;
}
