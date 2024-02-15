#include <stdio.h>
#include <stdlib.h>

#include "eval.h"
#include "utils.h"

void interpreter()
{
    char line[1024];

    struct Environment env = bootstrapEnvironment();

    for (;;)
    {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin))
        {
            printf("\n");
            break;
        }

        struct ExpressionLinkedList *expressions = sourceToExpressions(line);
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

    struct ExpressionLinkedList *expressions = sourceToExpressions(fileContents);
    struct Environment env = bootstrapEnvironment();

    while (expressions != NULL)
    {
        evaluate(&env, expressions->value);
        expressions = expressions->next;
    }

    free(fileContents);
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
