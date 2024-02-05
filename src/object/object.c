#include "dataTypes.h"

#include <stdio.h>
#include <stdlib.h>

struct Object booleanObject(bool boolean)
{
    struct Object object;
    object.kind = BOOLEAN;
    object.value.boolean = boolean;
    return object;
}

struct Object characterObject(char character)
{
    struct Object object;
    object.kind = CHARACTER;
    object.value.character = character;
    return object;
}

struct Object identifierObject(char *identifier)
{
    struct Object object;
    object.kind = IDENTIFIER;
    object.value.identifier = identifier;
    return object;
}

struct Object pairObject(struct Pair *pair)
{
    struct Object object;
    object.kind = PAIR;
    object.value.pair = pair;
    return object;
}

struct Object numberObject(double number)
{
    struct Object object;
    object.kind = NUMBER;
    object.value.number = number;
    return object;
}

struct Object procedureObject(struct Procedure procedure)
{
    struct Object object;
    object.kind = PROCEDURE;
    object.value.procedure = procedure;
    return object;
}

struct Object stringObject(char *string)
{
    struct Object object;
    object.kind = STRING;
    object.value.string = string;
    return object;
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

bool isTruthy(struct Object *object)
{
    return object->kind != BOOLEAN || object->value.boolean;
}

bool isNumber(struct Object *object)
{
    return object->kind == NUMBER;
}

bool isList(struct Object *object)
{
    return (object->kind == PAIR) && (object->value.pair == NULL || isList(&object->value.pair->cdr));
}

bool isPair(struct Object *object)
{
    return (object->kind == PAIR) && (object->value.pair != NULL) && !isList(&object->value.pair->cdr);
}

void printObject(struct Object *object);

void printPair(struct Pair *pair)
{
    printf("(");
    printObject(&pair->car);
    printf(" . ");
    printObject(&pair->cdr);
    printf(")");
}

void printList(struct Pair *list)
{
    printf("(");
    if (list != NULL)
    {
        printObject(&list->car);

        list = list->cdr.value.pair;
        while (list != NULL)
        {
            printf(" ");
            printObject(&list->car);
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

void printObject(struct Object *object)
{
    switch (object->kind)
    {
    case PAIR:
        if (isPair(object))
        {
            printPair(object->value.pair);
        }
        else
        {
            printList(object->value.pair);
        }
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
    case PROCEDURE:
        printProcedure(object->value.procedure);
        break;
    case PRIMITIVE_PROCEDURE:
        printf("<primitive procedure>");
        break;
    case STRING:
        printf("\"%s\"", object->value.string);
        break;
    }
}
