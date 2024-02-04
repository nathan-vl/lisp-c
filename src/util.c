#include <stdio.h>
#include "util.h"

size_t listLength(Pair *pair)
{
    size_t length = 0;
    while (pair != NULL)
    {
        length++;
        pair = pair->cdr.value.pair;
    }
    return length;
}

bool isTruthy(Object *object)
{
    return object->kind != BOOLEAN || object->value.boolean;
}

bool isNumber(Object *object)
{
    return object->kind == NUMBER;
}

bool isList(Object *object)
{
    return (object->kind == PAIR) && (object->value.pair == NULL || isList(&object->value.pair->cdr));
}

bool isPair(Object *object)
{
    return (object->kind == PAIR) && (object->value.pair != NULL) && !isList(&object->value.pair->cdr);
}

void printObject(Object *object);

void printPair(Pair *pair)
{
    printf("(");
    printObject(&pair->car);
    printf(" . ");
    printObject(&pair->cdr);
    printf(")");
}

void printList(Pair *list)
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

void printObject(Object *object)
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
    case PRIMITIVE_PROCEDURE:
        printf("<proc>");
        break;
    case STRING:
        printf("\"%s\"", object->value.string);
        break;
    }
}
