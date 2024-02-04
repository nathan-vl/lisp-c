#include <stdio.h>
#include <stdlib.h>

#include "error.h"

void checkArityError(size_t expected, size_t actual)
{
    if (expected != actual)
    {
        printf("Error. Arity difference in procedure. Expected %ld but got %ld.\n", expected, actual);
        exit(-1);
    }
}
