#ifndef PARSER_DATA_TYPES_H
#define PARSER_DATA_TYPES_H

#include "object.h"

struct ObjectLinkedList
{
    struct Object value;
    struct ObjectLinkedList *next;
};

#endif
