#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

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

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        interpreter();
    }
    else if (argc == 2)
    {
        TokenLinkedList *current = parse("(+ 2 2)");

        while (current != NULL)
        {
            printToken(current->token);
            freeToken(current->token);

            TokenLinkedList *old = current;
            current = current->next;
            free(old);
        };
    }
    else
    {
        // Error
    }

    return 0;
}
