#include "token.h"

#include <stdlib.h>

struct Token newToken(enum TokenType type, char *lexeme, size_t line, size_t col)
{
    struct Token token;
    token.type = type;
    token.lexeme = lexeme;
    token.line = line;
    token.col = col;
    return token;
}

struct Token newStringToken(char *string, size_t line, size_t col)
{
    struct Token token = newToken(T_STRING, string, line, col);
    token.literal = newStringLiteral(string);
    return token;
}

struct Token newNumberToken(char *strNumber, size_t line, size_t col)
{
    struct Token token = newToken(T_NUMBER, NULL, line, col);
    token.literal = newNumberLiteral(atof(strNumber));
    free(strNumber);
    return token;
}
