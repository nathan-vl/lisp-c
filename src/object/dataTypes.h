#ifndef OBJECT_DATATYPES_H
#define OBJECT_DATATYPES_H

#include <stdbool.h>
#include <stdlib.h>

struct Procedure
{
    char **parameters;
    size_t parametersLength;
    struct Pair *body;
};

enum ObjectKind
{
    BOOLEAN,
    CHARACTER,
    IDENTIFIER,
    PAIR,
    NUMBER,
    PRIMITIVE_PROCEDURE,
    PROCEDURE,
    STRING,
};

union ObjectValue
{
    bool boolean;
    char character;
    char *identifier;
    struct Pair *pair;
    struct Procedure procedure;
    void *primitiveProcedure; // Points to function type : Object procedure(Environment*, Pair*)
    double number;
    char *string;
};

struct Object
{
    enum ObjectKind kind;
    union ObjectValue value;
};

struct Pair
{
    struct Object car;
    struct Object cdr;
};

#endif
