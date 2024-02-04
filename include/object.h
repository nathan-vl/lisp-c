#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct Procedure
{
    char **parameters;
    size_t parametersLength;
    struct Pair *body;
} Procedure;

typedef enum ObjectKind
{
    BOOLEAN,
    CHARACTER,
    IDENTIFIER,
    PAIR,
    NUMBER,
    PRIMITIVE_PROCEDURE,
    PROCEDURE,
    STRING,
} ObjectKind;

typedef union ObjectValue
{
    bool boolean;
    char character;
    char *identifier;
    struct Pair *pair;
    struct Procedure procedure;
    void *primitiveProcedure; // Points to function type : Object procedure(Environment*, Pair*)
    double number;
    char *string;
} ObjectValue;

typedef struct Object
{
    ObjectKind kind;
    ObjectValue value;
} Object;

typedef struct Pair
{
    Object car;
    Object cdr;
} Pair;

Object booleanObject(bool boolean);
Object characterObject(char character);
Object identifierObject(char *identifier);
Object pairObject(struct Pair *pair);
Object numberObject(double number);
Object procedureObject(Procedure procedure);
Object stringObject(char *string);

#endif
