#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "object.h"
#include "dataTypes.h"

struct ObjectLinkedList *syntaxAnalyser(struct TokenLinkedList *tokens);

#endif
