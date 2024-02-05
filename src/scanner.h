#ifndef SCANNER_H
#define SCANNER_H

#include "scanner/dataTypes.h"

struct TokenLinkedList *parse(char *source);
void freeToken(struct Token token);

#endif
