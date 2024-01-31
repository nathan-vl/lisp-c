#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "eval.h"

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
                printf("%s", literal.value.boolean ? "t" : "nil");
                break;
            case NUMBER:
                printf("%f", literal.value.number);
                break;
            case STRING:
                printf("\"%s\"", literal.value.string);
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
        Object objectEval = evaluate(object);
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
