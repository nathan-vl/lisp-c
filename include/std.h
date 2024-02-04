#ifndef STD_H
#define STD_H

#include <stdio.h>
#include <stdlib.h>

#include "environment.h"
#include "error.h"
#include "eval.h"
#include "object.h"
#include "util.h"

/* Base */
Object cons(struct Environment *env, struct Pair *args);
Object define(struct Environment *env, struct Pair *args);
Object lambda(struct Environment *env, struct Pair *args);
Object print(struct Environment *env, struct Pair *args);
Object quote(struct Environment *env, struct Pair *pair);

/* Bool */
Object negation(struct Environment *env, struct Pair *args);

/* Math */
Object add(struct Environment *env, struct Pair *args);
Object subtract(struct Environment *env, struct Pair *args);
Object multiply(struct Environment *env, struct Pair *args);
Object divide(struct Environment *env, struct Pair *args);

#endif
