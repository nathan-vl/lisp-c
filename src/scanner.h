#pragma once

#include "scanner/dataTypes.h"

struct TokenLinkedList *parse(char *source);
void freeToken(struct Token token);
