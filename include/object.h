#ifndef OBJECT_H
#define OBJECT_H

#include "atom.h"

typedef enum ObjectKind
{
    ATOM,
    CONS
} ObjectKind;

typedef union ObjectValue
{
    struct Cons *cons;
    Atom atom;
} ObjectValue;

typedef struct Object
{
    ObjectKind kind;
    ObjectValue value;
} Object;

typedef struct Cons
{
    Object car;
    struct Cons *cdr;
} Cons;

#endif
