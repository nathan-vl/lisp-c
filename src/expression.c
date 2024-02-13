#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "expression.h"

struct Expression booleanExpression(bool boolean)
{
    struct Expression expression;
    expression.kind = BOOLEAN;
    expression.value.boolean = boolean;
    return expression;
}

struct Expression characterExpression(char character)
{
    struct Expression expression;
    expression.kind = CHARACTER;
    expression.value.character = character;
    return expression;
}

struct Expression symbolExpression(char *symbol)
{
    struct Expression expression;
    expression.kind = SYMBOL;
    expression.value.symbol = symbol;
    return expression;
}

struct Expression listExpression(struct List *list)
{
    struct Expression expression;
    expression.kind = LIST;
    expression.value.list = list;
    return expression;
}

struct Expression numberExpression(double number)
{
    struct Expression expression;
    expression.kind = NUMBER;
    expression.value.number = number;
    return expression;
}

struct Expression procedureExpression(struct Procedure procedure)
{
    struct Expression expression;
    expression.kind = PROCEDURE;
    expression.value.procedure = procedure;
    return expression;
}

struct Expression stringExpression(char *string)
{
    struct Expression expression;
    expression.kind = STRING;
    expression.value.string = string;
    return expression;
}

size_t listLength(struct List *list)
{
    size_t length = 0;
    while (list != NULL)
    {
        length++;
        list = list->cdr;
    }
    return length;
}

bool isTruthy(struct Expression *expression)
{
    return expression->kind != BOOLEAN || expression->value.boolean;
}

bool isNumber(struct Expression *expression)
{
    return expression->kind == NUMBER;
}

bool isList(struct Expression *expression)
{
    return expression->kind == LIST;
}

void printExpression(struct Expression *expression);

void printList(struct List *list)
{
    printf("(");
    if (list != NULL)
    {
        printExpression(&list->car);

        list = list->cdr;
        while (list != NULL)
        {
            printf(" ");
            printExpression(&list->car);
            list = list->cdr;
        }
    }
    printf(")");
}

void printProcedure(struct Procedure procedure)
{
    printf("(lambda ");

    // Symbols
    printf("(");
    if (procedure.parametersLength > 0)
    {
        printf("%s", procedure.parameters[0]);
    }
    for (size_t i = 1; i < procedure.parametersLength; i++)
    {
        printf(" %s", procedure.parameters[i]);
    }
    printf(") ");

    printList(procedure.body);

    printf(")");
}

void printExpression(struct Expression *expression)
{
    switch (expression->kind)
    {
    case LIST:
        printList(expression->value.list);
        break;
    case BOOLEAN:
        printf("%s", expression->value.boolean ? "#t" : "#f");
        break;
    case CHARACTER:
        printf("%c", expression->value.character);
        break;
    case SYMBOL:
        printf("%s", expression->value.symbol);
        break;
    case NUMBER:
        printf("%f", expression->value.number);
        break;
    case PROCEDURE:
        printProcedure(expression->value.procedure);
        break;
    case PRIMITIVE_PROCEDURE:
        printf("<primitive procedure>");
        break;
    case STRING:
        printf("\"%s\"", expression->value.string);
        break;
    }
}
