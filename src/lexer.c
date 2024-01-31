#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool isValidIdentifier(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_') || (c == '+') || (c == '-') || (c == '*') || (c == '/');
}

Token newToken(enum TokenType type, char *lexeme)
{
    Token token;
    token.type = type;
    token.lexeme = lexeme;
    return token;
}

Token newStringToken(char *string)
{
    Token token = newToken(T_STRING, string);
    token.literal = newStringLiteral(string);
    return token;
}

Token newNumberToken(char *strNumber)
{
    Token token = newToken(T_NUMBER, NULL);
    token.literal = newNumberLiteral(atof(strNumber));
    free(strNumber);
    return token;
}

void printToken(Token token)
{
    switch (token.type)
    {
    case T_F:
        printf("#f\n");
        break;
    case T_T:
        printf("#t\n");
        break;
    case T_OPEN_PAREN:
        printf("L_PAREN\n");
        break;
    case T_CLOSE_PAREN:
        printf("R_PAREN\n");
        break;
    case T_IDENTIFIER:
        printf("<IDENT %s>\n", token.lexeme);
        break;
    case T_STRING:
        printf("<STR \"%s\">\n", token.literal.value.string);
        break;
    case T_NUMBER:
        printf("<NUM %f>\n", token.literal.value.number);
        break;
    default:
        printf("<UNDEFINED>\n");
    }
}

Token parseString(LexerStatus *status)
{
    status->source++;
    char *start = status->source;
    while (status->source[0] != '\"')
    {
        if (status->source[0] == '\0')
        {
            printf("Error. Unterminated string\n");
            exit(-1);
        }
        status->source++;
    }

    size_t length = (status->source - start) / sizeof(char) + 1; // + 1 for the \0
    char *copy = malloc(sizeof(char) * length);
    strncpy(copy, start, length);
    copy[length - 1] = '\0';

    return newStringToken(copy);
}

Token parseNumber(LexerStatus *status)
{
    char *start = status->source;
    while (isDigit(status->source[0]))
    {
        status->source++;
    }

    if (status->source[0] == '.' && isDigit(status->source[1]))
    {
        status->source++;
        while (isDigit(status->source[0]))
        {
            status->source++;
        }
    }

    size_t length = (status->source - start) / sizeof(char) + 1; // +1 for the '\0'
    char *copy = malloc(sizeof(char) * length);
    strncpy(copy, start, length);
    copy[length - 1] = '\0';
    return newNumberToken(copy);
}

TokenType identifierType(char *identifier)
{
    if (strcmp(identifier, "f") == 0)
        return T_F;
    if (strcmp(identifier, "t") == 0)
        return T_T;
    return T_IDENTIFIER;
}

Token parseIdentifier(LexerStatus *status)
{
    char *start = status->source;

    // Since the first character is read, it can be ignored
    status->source++;

    while (isValidIdentifier(status->source[0]) || isDigit(status->source[0]))
    {
        status->source++;
    }

    size_t length = (status->source - start) / sizeof(char) + 1; // + 1 for the \0
    char *copy = malloc(sizeof(char) * length);
    strncpy(copy, start, length);
    copy[length - 1] = '\0';

    TokenType type = identifierType(copy);
    switch (type)
    {
    case T_F:
    case T_T:
        free(copy);
        return newToken(type, NULL);
    default:
        return newToken(type, copy);
    }
}

// TODO: Check if tokens literals should be free'd now
void freeToken(Token token)
{
    switch (token.type)
    {
    case T_OPEN_PAREN:
    case T_CLOSE_PAREN:
    case T_NUMBER:
        break;
    default:
        free(token.lexeme);
        break;
    }
}

Token parseCharacter(LexerStatus *status)
{
    char *start = status->source;
    while (isValidIdentifier(status->source[0]))
    {
        status->source++;
    }

    size_t length = (status->source - start) / sizeof(char);
    if (length == 1)
    {
        Token token = newToken(T_CHARACTER, NULL);
        token.literal.kind = L_CHARACTER;
        token.literal.value.character = start[0];
        return token;
    }

    char *copy = malloc(sizeof(char) * length + 1);
    strncpy(copy, start, length);
    copy[length] = '\0';

    Token token = newToken(T_CHARACTER, NULL);
    if (strcmp(copy, "newline") == 0)
    {
        token.literal.kind = L_CHARACTER;
        token.literal.value.character = '\n';
    }
    else if (strcmp(copy, "space") == 0)
    {
        token.literal.kind = L_CHARACTER;
        token.literal.value.character = ' ';
    }
    else if (strcmp(copy, "tab") == 0)
    {
        token.literal.kind = L_CHARACTER;
        token.literal.value.character = '\t';
    }
    else
    {
        printf("Error. Character identifier \"#\\%s\" not recognized.", copy);
        free(copy);
        exit(-1);
    }

    free(copy);
    return token;
}

Token parseHash(LexerStatus *status)
{
    // Ignore hash character
    status->source++;

    Token token;
    switch (status->source[0])
    {
    case '\\':
        status->source++;
        return parseCharacter(status);
        break;
    case 'f':
    case 'F':
        status->source++;
        token = newToken(T_F, NULL);
        token.literal.kind = L_BOOLEAN;
        token.literal.value.boolean = false;
        return token;
    case 't':
    case 'T':
        status->source++;
        token = newToken(T_T, NULL);
        token.literal.kind = L_BOOLEAN;
        token.literal.value.boolean = true;
        return token;
    default:
        printf("Error. Unrecognized character after '#'\n");
        exit(-1);
    }
}

Token parseToken(LexerStatus *status)
{
    char c = status->source[0];
    switch (c)
    {
    case '\0':
        status->source++;
        return newToken(T_EOF, "\0");
    case '(':
        status->source++;
        return newToken(T_OPEN_PAREN, "(");
    case ')':
        status->source++;
        return newToken(T_CLOSE_PAREN, ")");
    case '"':
        return parseString(status);
    case '#':
        return parseHash(status);

    // Ignore whitespace
    case '\n':
    case ' ':
    case '\r':
    case '\t':
        status->source++;
        return newToken(T_WHITESPACE, NULL);

    default:
        if (isDigit(c))
        {
            return parseNumber(status);
        }
        else if (isValidIdentifier(c))
        {
            return parseIdentifier(status);
        }
        else
        {
            printf("Error. Character '%c' not recognized\n", c);
            exit(-1);
        }
    }
}

TokenLinkedList *parse(char *source)
{
    TokenLinkedList tokensHead;
    TokenLinkedList *current = &tokensHead;

    LexerStatus status;
    status.source = source;

    while (status.source[0] != '\0')
    {
        Token token = parseToken(&status);
        if (token.type == T_WHITESPACE)
        {
            continue;
        }

        current->next = malloc(sizeof(TokenLinkedList));
        current = current->next;
        current->token = token;
        current->next = NULL;
    }

    return tokensHead.next;
}
