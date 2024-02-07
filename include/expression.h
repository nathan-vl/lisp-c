#pragma once

#include <stdbool.h>
#include <stdlib.h>

struct Procedure
{
    char **parameters;
    size_t parametersLength;
    struct Pair *body;
};

enum ExpressionKind
{
    BOOLEAN,
    CHARACTER,
    IDENTIFIER,
    PAIR,
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
    struct Pair *pair;
    struct Procedure procedure;
    void *primitiveProcedure; // Points to function type : Expression procedure(Environment*, Pair*)
    double number;
    char *string;
};

struct Expression
{
    enum ExpressionKind kind;
    union ExpressionValue value;
};

struct Pair
{
    struct Expression car;
    struct Expression cdr;
};

struct Expression booleanExpression(bool boolean);
struct Expression characterExpression(char character);
struct Expression identifierExpression(char *identifier);
struct Expression pairExpression(struct Pair *pair);
struct Expression numberExpression(double number);
struct Expression procedureExpression(struct Procedure procedure);
struct Expression stringExpression(char *string);

size_t listLength(struct Pair *pair);

bool isTruthy(struct Expression *expression);
bool isNumber(struct Expression *expression);
bool isList(struct Expression *expression);
bool isPair(struct Expression *expression);
