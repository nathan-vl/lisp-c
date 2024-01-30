#include "literal.h"

Literal newStringLiteral(char *string)
{
    Literal literal;
    literal.type = STRING;
    literal.value.string = string;
    return literal;
}

Literal newNumberLiteral(double number)
{
    Literal literal;
    literal.type = NUMBER;
    literal.value.number = number;
    return literal;
}
