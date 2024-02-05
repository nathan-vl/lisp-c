#ifndef LITERAL_H
#define LITERAL_H

#include <stdbool.h>

enum LiteralKind
{
    L_BOOLEAN,
    L_CHARACTER,
    L_NUMBER,
    L_STRING,
};

union LiteralValue
{
    bool boolean;
    char character;
    double number;
    char *string;
};

struct Literal
{
    enum LiteralKind kind;
    union LiteralValue value;
};

struct Literal newStringLiteral(char *string);
struct Literal newNumberLiteral(double number);
struct Literal newFalseLiteral();
struct Literal newTrueLiteral();

#endif
