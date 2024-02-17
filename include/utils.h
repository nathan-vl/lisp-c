#pragma once

char *readFile(char *path);
void includeFile(struct Environment *environment, char *path);
struct Expression deepCopy(const struct Expression *expression);
struct Expression replaceMacro(struct Environment *env, struct Macro macro, struct List *args);
struct ExpressionLinkedList *sourceToExpressions(char *source);
