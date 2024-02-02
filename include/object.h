#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>

typedef struct Procedure
{
    struct List *parameters;
    struct List *body;
} Procedure;

typedef enum ObjectKind
{
    BOOLEAN,
    CHARACTER,
    IDENTIFIER,
    LIST,
    NUMBER,
    PROCEDURE,
    STRING,
} ObjectKind;

typedef union ObjectValue
{
    bool boolean;
    char character;
    char *identifier;
    struct List *list;
    struct Procedure procedure;
    double number;
    char *string;
} ObjectValue;

typedef struct Object
{
    ObjectKind kind;
    ObjectValue value;
} Object;

typedef struct List
{
    Object car;
    struct List *cdr;
} List;

Object booleanObject(bool boolean);
Object characterObject(char character);
Object identifierObject(char *identifier);
Object listObject(List *list);
Object numberObject(double number);
Object procedureObject(Procedure procedure);
Object stringObject(char *string);

#endif
