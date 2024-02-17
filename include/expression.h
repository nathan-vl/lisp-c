#pragma once

#include <stdbool.h>
#include <stdlib.h>

struct Procedure
{
    char **parameters;
    size_t parametersLength;
    struct Expression *body;
};

enum ExpressionKind
{
    BOOLEAN,
    CHARACTER,
    LIST,
    MACRO,
    NUMBER,
    PRIMITIVE_PROCEDURE,
    PROCEDURE,
    STRING,
    SYMBOL,
};

union ExpressionValue
{
    bool boolean;
    char character;
    char *symbol;
    struct List *list;
    struct Macro *macro;
    double number;
    void *primitiveProcedure; // Points to function type : Expression procedure(Environment*, List*)
    struct Procedure procedure;
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

struct Macro
{
    char **parameters;
    size_t parametersLength;
    struct Expression body;
};

struct Expression booleanExpression(bool boolean);
struct Expression characterExpression(char character);
struct Expression symbolExpression(char *symbol);
struct Expression listExpression(struct List *list);
struct Expression macroExpression(struct Macro macro);
struct Expression numberExpression(double number);
struct Expression procedureExpression(struct Procedure procedure);
struct Expression stringExpression(char *string);

size_t listLength(struct List *list);

bool isTruthy(struct Expression expression);
bool isNumber(struct Expression *expression);
bool isList(struct Expression *expression);
bool isList(struct Expression *expression);
