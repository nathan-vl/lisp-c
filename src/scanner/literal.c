#include "literal.h"

struct Literal newStringLiteral(char *string)
{
    struct Literal literal;
    literal.kind = L_STRING;
    literal.value.string = string;
    return literal;
}

struct Literal newNumberLiteral(double number)
{
    struct Literal literal;
    literal.kind = L_NUMBER;
    literal.value.number = number;
    return literal;
}

struct Literal newFalseLiteral()
{
    struct Literal literal;
    literal.kind = L_BOOLEAN;
    literal.value.boolean = false;
    return literal;
}

struct Literal newTrueLiteral()
{
    struct Literal literal;
    literal.kind = L_BOOLEAN;
    literal.value.boolean = true;
    return literal;
}
