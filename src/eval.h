#pragma once

#include "parser.h"
#include "environment.h"

void printObject(struct Object *object);
struct Object evaluate(struct Environment *env, struct Object object);

void checkArityError(size_t expected, size_t actual);
