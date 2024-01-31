#ifndef LITERAL_H
#define LITERAL_H

#include <stdbool.h>

typedef enum LiteralKind
{
    L_BOOLEAN,
    L_CHARACTER,
    L_NUMBER,
    L_STRING,
} LiteralKind;

typedef union LiteralValue
{
    bool boolean;
    char character;
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
Literal newFalseLiteral();
Literal newTrueLiteral();

#endif
