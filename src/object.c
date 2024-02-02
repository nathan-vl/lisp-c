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

Object pairObject(Pair *pair)
{
    Object object;
    object.kind = PAIR;
    object.value.pair = pair;
    return object;
}

Object numberObject(double number)
{
    Object object;
    object.kind = NUMBER;
    object.value.number = number;
    return object;
}

Object procedureObject(Procedure procedure)
{
    Object object;
    object.kind = PROCEDURE;
    object.value.procedure = procedure;
    return object;
}

Object stringObject(char *string)
{
    Object object;
    object.kind = STRING;
    object.value.string = string;
    return object;
}
