#include "token.h"

#include <stdlib.h>

struct Token newToken(enum TokenType type, char *lexeme)
{
    struct Token token;
    token.type = type;
    token.lexeme = lexeme;
    return token;
}

struct Token newStringToken(char *string)
{
    struct Token token = newToken(T_STRING, string);
    token.literal = newStringLiteral(string);
    return token;
}

struct Token newNumberToken(char *strNumber)
{
    struct Token token = newToken(T_NUMBER, NULL);
    token.literal = newNumberLiteral(atof(strNumber));
    free(strNumber);
    return token;
}
