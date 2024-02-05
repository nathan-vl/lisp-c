#pragma once

#include "object.h"

struct ObjectLinkedList
{
    struct Object value;
    struct ObjectLinkedList *next;
};
