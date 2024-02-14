#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "environment.h"
#include "eval.h"
#include "expression.h"

// Core
struct Expression cons(struct Environment *env, struct List *args);
struct Expression define(struct Environment *env, struct List *args);
struct Expression defmacro(struct Environment *env, struct List *args);
struct Expression ifExpr(struct Environment *env, struct List *args);
struct Expression lambda(struct Environment *env, struct List *args);
struct Expression print(struct Environment *env, struct List *args);
struct Expression quote(struct Environment *env, struct List *list);
struct Expression setValue(struct Environment *env, struct List *args);
struct Expression whileExpr(struct Environment *env, struct List *args);

// Bool
struct Expression equals(struct Environment *env, struct List *args);
struct Expression lessThan(struct Environment *env, struct List *args);
struct Expression greaterThan(struct Environment *env, struct List *args);
struct Expression lessEqualThan(struct Environment *env, struct List *args);
struct Expression greaterEqualThan(struct Environment *env, struct List *args);
struct Expression negation(struct Environment *env, struct List *args);
struct Expression andExpr(struct Environment *env, struct List *args);
struct Expression orExpr(struct Environment *env, struct List *args);

// Math
struct Expression add(struct Environment *env, struct List *args);
struct Expression subtract(struct Environment *env, struct List *args);
struct Expression multiply(struct Environment *env, struct List *args);
struct Expression divide(struct Environment *env, struct List *args);
struct Expression exponentiation(struct Environment *env, struct List *args);
struct Expression modulo(struct Environment *env, struct List *args);
