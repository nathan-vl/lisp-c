#include "literal.h"

Literal newStringLiteral(char *string)
{
    Literal literal;
    literal.kind = L_STRING;
    literal.value.string = string;
    return literal;
}

Literal newNumberLiteral(double number)
{
    Literal literal;
    literal.kind = L_NUMBER;
    literal.value.number = number;
    return literal;
}

Literal newFalseLiteral()
{
    Literal literal;
    literal.kind = L_BOOLEAN;
    literal.value.boolean = false;
    return literal;
}

Literal newTrueLiteral()
{
    Literal literal;
    literal.kind = L_BOOLEAN;
    literal.value.boolean = true;
    return literal;
}
