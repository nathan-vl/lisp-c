#ifndef ATOM_H
#define ATOM_H

#include "literal.h"

typedef enum AtomKind
{
    IDENTIFIER,
    LITERAL,
} AtomKind;

typedef union AtomValue
{
    char *identifier;
    Literal literal;
} AtomValue;

typedef struct Atom
{
    AtomKind kind;
    AtomValue value;
} Atom;

Atom literalToAtom(Literal literal);
Atom identifierToAtom(char *identifier);

#endif
