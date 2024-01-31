#ifndef EVAL_H
#define EVAL_H

#include "object.h"
#include "environment.h"

Object evaluate(Environment *env, Object object);

#endif
