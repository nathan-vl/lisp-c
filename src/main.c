#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

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

        TokenLinkedList *current = parse(line);
        while (current != NULL)
        {
            printToken(current->token);
            freeToken(current->token);

            TokenLinkedList *old = current;
            current = current->next;
            free(old);
        };
    }
}

void printObject(Object *object)
{
    switch (object->kind)
    {
    case ATOM:
        Atom atom = object->value.atom;
        switch (atom.kind)
        {
        case IDENTIFIER:
            char *identifier = atom.value.identifier;
            printf("%s", identifier);
            break;
        case LITERAL:
            Literal literal = atom.value.literal;
            switch (literal.kind)
            {
            case BOOLEAN:
                printf("%d", literal.value.boolean);
                break;
            case NUMBER:
                printf("%f", literal.value.number);
                break;
            case STRING:
                printf("%s", literal.value.string);
                break;
            }
        }
        break;
    case CONS:
        Cons *cons = object->value.cons;
        printf("(");
        if (cons != NULL)
        {
            printObject(&cons->car);

            cons = cons->cdr;
            while (cons != NULL)
            {
                printf(" ");
                printObject(&cons->car);
                cons = cons->cdr;
            }
        }
        printf(")");
    }
}

int main(int argc, char **argv)
{
    TokenLinkedList *current = parse("(+ 2 2)");
    Object object = syntaxAnalyser(current);

    printObject(&object);
    printf("\n");

    return 0;
}
