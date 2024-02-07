#pragma once

#include "parser.h"
#include "environment.h"

void printExpression(struct Expression *expression);
struct Expression evaluate(struct Environment *env, struct Expression expression);

void checkArityError(size_t expected, size_t actual);
