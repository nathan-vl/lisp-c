#pragma once

char *readFile(char *path);
void includeFile(struct Environment *environment, char *path);
struct ExpressionLinkedList *sourceToExpressions(char *source);
