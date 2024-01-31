#include "object.h"

Object consToObject(Cons *cons)
{
    Object object;
    object.kind = CONS;
    object.value.cons = cons;
    return object;
}

Object atomToObject(Atom atom)
{
    Object object;
    object.kind = ATOM;
    object.value.atom = atom;
    return object;
}
