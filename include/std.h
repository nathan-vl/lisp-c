#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "environment.h"
#include "eval.h"
#include "expression.h"

// Base
struct Expression cons(struct Environment *env, struct List *args);
struct Expression define(struct Environment *env, struct List *args);
struct Expression lambda(struct Environment *env, struct List *args);
struct Expression print(struct Environment *env, struct List *args);
struct Expression quote(struct Environment *env, struct List *list);

// Bool
struct Expression negation(struct Environment *env, struct List *args);

// Math
struct Expression add(struct Environment *env, struct List *args);
struct Expression subtract(struct Environment *env, struct List *args);
struct Expression multiply(struct Environment *env, struct List *args);
struct Expression divide(struct Environment *env, struct List *args);
