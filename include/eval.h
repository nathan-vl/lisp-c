#ifndef EVAL_H
#define EVAL_H

#include "object.h"
#include "environment.h"

void printObject(Object *object);
Object evaluate(Environment *env, Object object);

#endif
