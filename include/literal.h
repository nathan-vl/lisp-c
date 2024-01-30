#ifndef LITERAL_H
#define LITERAL_H

#include <stdbool.h>

typedef enum DataType
{
    BOOLEAN,
    NUMBER,
    STRING
} DataType;

typedef union Value
{
    bool boolean;
    double number;
    char *string;
} Value;

typedef struct Literal
{
    DataType type;
    Value value;
} Literal;

Literal newStringLiteral(char *string);
Literal newNumberLiteral(double number);

#endif
