#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "environment.h"
#include "eval.h"
#include "object.h"

// Base
struct Object cons(struct Environment *env, struct Pair *args);
struct Object define(struct Environment *env, struct Pair *args);
struct Object lambda(struct Environment *env, struct Pair *args);
struct Object print(struct Environment *env, struct Pair *args);
struct Object quote(struct Environment *env, struct Pair *pair);

// Bool
struct Object negation(struct Environment *env, struct Pair *args);

// Math
struct Object add(struct Environment *env, struct Pair *args);
struct Object subtract(struct Environment *env, struct Pair *args);
struct Object multiply(struct Environment *env, struct Pair *args);
struct Object divide(struct Environment *env, struct Pair *args);
