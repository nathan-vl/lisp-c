#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include "object/dataTypes.h"

struct Object booleanObject(bool boolean);
struct Object characterObject(char character);
struct Object identifierObject(char *identifier);
struct Object pairObject(struct Pair *pair);
struct Object numberObject(double number);
struct Object procedureObject(struct Procedure procedure);
struct Object stringObject(char *string);

size_t listLength(struct Pair *pair);

bool isTruthy(struct Object *object);
bool isNumber(struct Object *object);
bool isList(struct Object *object);
bool isPair(struct Object *object);
