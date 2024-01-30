#include "atom.h"

Atom literalToAtom(Literal literal)
{
    Atom atom;
    atom.kind = LITERAL;
    atom.value.literal = literal;
    return atom;
}

Atom identifierToAtom(char *identifier)
{
    Atom atom;
    atom.kind = IDENTIFIER;
    atom.value.identifier = identifier;
    return atom;
}
