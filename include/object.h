#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>

typedef struct Procedure
{
    struct Pair *parameters;
    struct Pair *body;
} Procedure;

typedef enum ObjectKind
{
    BOOLEAN,
    CHARACTER,
    IDENTIFIER,
    PAIR,
    NUMBER,
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
