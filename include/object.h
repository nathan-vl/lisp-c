#pragma once

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

struct Object booleanObject(bool boolean);
struct Object characterObject(char character);
struct Object identifierObject(char *identifier);
struct Object pairObject(struct Pair *pair);
struct Object numberObject(double number);
struct Object procedureObject(struct Procedure procedure);
struct Object stringObject(char *string);

size_t listLength(struct Pair *pair);

bool isTruthy(struct Object *object);
bool isNumber(struct Object *object);
bool isList(struct Object *object);
bool isPair(struct Object *object);
