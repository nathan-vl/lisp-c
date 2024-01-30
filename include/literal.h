#ifndef LITERAL_H
#define LITERAL_H

#include <stdbool.h>

typedef enum LiteralKind
{
    BOOLEAN,
    NUMBER,
    STRING
} LiteralKind;

typedef union LiteralValue
{
    bool boolean;
    double number;
    char *string;
} LiteralValue;

typedef struct Literal
{
    LiteralKind kind;
    LiteralValue value;
} Literal;

Literal newStringLiteral(char *string);
Literal newNumberLiteral(double number);

#endif
