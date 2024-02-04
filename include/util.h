#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdlib.h>
#include "object.h"

size_t listLength(Pair *pair);

bool isTruthy(Object *object);
bool isNumber(Object *object);
bool isList(Object *object);
bool isPair(Object *object);

#endif
