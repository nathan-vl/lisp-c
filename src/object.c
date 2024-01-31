#include "object.h"

Object booleanObject(bool boolean)
{
    Object object;
    object.kind = BOOLEAN;
    object.value.boolean = boolean;
    return object;
}

Object characterObject(char character)
{
    Object object;
    object.kind = CHARACTER;
    object.value.character = character;
    return object;
}

Object identifierObject(char *identifier)
{
    Object object;
    object.kind = IDENTIFIER;
    object.value.identifier = identifier;
    return object;
}

Object listObject(List *list)
{
    Object object;
    object.kind = LIST;
    object.value.list = list;
    return object;
}

Object numberObject(double number)
{
    Object object;
    object.kind = NUMBER;
    object.value.number = number;
    return object;
}

Object stringObject(char *string)
{
    Object object;
    object.kind = STRING;
    object.value.string = string;
    return object;
}
