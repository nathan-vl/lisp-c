#pragma once

#include <stdbool.h>
#include <stdlib.h>

struct Procedure
{
    char **parameters;
    size_t parametersLength;
    struct List *body;
};

enum ExpressionKind
{
    BOOLEAN,
    CHARACTER,
    IDENTIFIER,
    LIST,
    NUMBER,
    PRIMITIVE_PROCEDURE,
    PROCEDURE,
    STRING,
};

union ExpressionValue
{
    bool boolean;
    char character;
    char *identifier;
    struct List *list;
    struct Procedure procedure;
    void *primitiveProcedure; // Points to function type : Expression procedure(Environment*, List*)
    double number;
    char *string;
};

struct Expression
{
    enum ExpressionKind kind;
    union ExpressionValue value;
};

struct List
{
    struct Expression car;
    struct List *cdr;
};

struct Expression booleanExpression(bool boolean);
struct Expression characterExpression(char character);
struct Expression identifierExpression(char *identifier);
struct Expression listExpression(struct List *list);
struct Expression numberExpression(double number);
struct Expression procedureExpression(struct Procedure procedure);
struct Expression stringExpression(char *string);

size_t listLength(struct List *list);

bool isTruthy(struct Expression *expression);
bool isNumber(struct Expression *expression);
bool isList(struct Expression *expression);
bool isList(struct Expression *expression);
