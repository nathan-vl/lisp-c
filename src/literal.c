#include "literal.h"

Literal newStringLiteral(char *string)
{
    Literal literal;
    literal.kind = STRING;
    literal.value.string = string;
    return literal;
}

Literal newNumberLiteral(double number)
{
    Literal literal;
    literal.kind = NUMBER;
    literal.value.number = number;
    return literal;
}
