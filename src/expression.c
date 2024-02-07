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

struct Expression identifierExpression(char *identifier)
{
    struct Expression expression;
    expression.kind = IDENTIFIER;
    expression.value.identifier = identifier;
    return expression;
}

struct Expression pairExpression(struct Pair *pair)
{
    struct Expression expression;
    expression.kind = PAIR;
    expression.value.pair = pair;
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

size_t listLength(struct Pair *pair)
{
    size_t length = 0;
    while (pair != NULL)
    {
        length++;
        pair = pair->cdr.value.pair;
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
    return (expression->kind == PAIR) && (expression->value.pair == NULL || isList(&expression->value.pair->cdr));
}

bool isPair(struct Expression *expression)
{
    return (expression->kind == PAIR) && (expression->value.pair != NULL) && !isList(&expression->value.pair->cdr);
}

void printExpression(struct Expression *expression);

void printPair(struct Pair *pair)
{
    printf("(");
    printExpression(&pair->car);
    printf(" . ");
    printExpression(&pair->cdr);
    printf(")");
}

void printList(struct Pair *list)
{
    printf("(");
    if (list != NULL)
    {
        printExpression(&list->car);

        list = list->cdr.value.pair;
        while (list != NULL)
        {
            printf(" ");
            printExpression(&list->car);
            list = list->cdr.value.pair;
        }
    }
    printf(")");
}

void printProcedure(struct Procedure procedure)
{
    printf("(lambda ");

    // Identifiers
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
    case PAIR:
        if (isPair(expression))
        {
            printPair(expression->value.pair);
        }
        else
        {
            printList(expression->value.pair);
        }
        break;
    case BOOLEAN:
        printf("%s", expression->value.boolean ? "#t" : "#f");
        break;
    case CHARACTER:
        printf("%c", expression->value.character);
        break;
    case IDENTIFIER:
        printf("%s", expression->value.identifier);
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
