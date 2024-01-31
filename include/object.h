#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>

typedef enum ObjectKind
{
    BOOLEAN,
    CHARACTER,
    IDENTIFIER,
    LIST,
    NUMBER,
    STRING,
} ObjectKind;

typedef union ObjectValue
{
    bool boolean;
    char character;
    char *identifier;
    struct List *list;
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

/*
BOOLEAN,
CHARACTER,
IDENTIFIER,
LIST,
NUMBER,
STRING,
*/

Object booleanObject(bool boolean);
Object characterObject(char character);
Object identifierObject(char *identifier);
Object listObject(List *list);
Object numberObject(double number);
Object stringObject(char *string);

#endif
