#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#include "environment.h"
#include "eval.h"
#include "parser.h"
#include "scanner.h"
#include "utils.h"

char *readFile(char *path)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL)
    {
        printf("Error. Could not open file \"%s\".\n", path);
    }

    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(size + 1);
    if (buffer == NULL)
    {
        printf("Error. Not enough memory to read file.");
    }

    size_t bytesRead = fread(buffer, sizeof(char), size, file);
    if (bytesRead < size)
    {
        printf("Error. Could not read file \"%s\".\n", path);
    }
    buffer[size - 1] = '\0';

    fclose(file);

    return buffer;
}

struct Expression replaceMacroBody(struct Environment *env, struct Environment *macroArgs, struct Expression macroBody)
{
    if (macroBody.kind == SYMBOL)
    {
        struct Expression *macroArg = getVariable(macroArgs, macroBody.value.symbol);
        if (macroArg != NULL)
        {
            return *macroArg;
        }
    }

    struct Expression body = evaluate(env, macroBody);
    if (body.kind != LIST)
    {
        return body;
    }

    struct List *current = body.value.list;

    while (current != NULL)
    {
        current->car = replaceMacroBody(env, macroArgs, current->car);
        current = current->cdr;
    }

    return body;
}

struct List *deepCopyList(const struct List *list)
{
    if (list == NULL)
    {
        return NULL;
    }

    struct List *copy = malloc(sizeof(struct List));
    copy->car = deepCopy(&list->car);
    copy->cdr = deepCopyList(list->cdr);

    return copy;
}

char *copyString(const char *string)
{
    size_t len = strlen(string);
    char *copy = malloc(sizeof(char) * len + 1);
    strcpy(copy, string);
    copy[len] = '\0';
    return copy;
}

struct Expression deepCopy(const struct Expression *expression)
{
    // Procedures and other types found to be not required to make deep copies

    switch (expression->kind)
    {
    case LIST:
        return listExpression(deepCopyList(expression->value.list));
    case MACRO:
        struct Macro macro;
        macro.body = deepCopy(&expression->value.macro->body);
        return macroExpression(macro);
    case SYMBOL:
        return symbolExpression(copyString(expression->value.symbol));
    case STRING:
        return symbolExpression(copyString(expression->value.string));
    default:
        return *expression;
    }
}

struct Expression replaceMacro(struct Environment *env, struct Macro macro, struct List *args)
{
    checkArityError(macro.parametersLength, listLength(args));

    struct Environment macroArgsEnv = createEnvironment(NULL);

    struct List *current = args;
    for (size_t i = 0; i < macro.parametersLength; i++)
    {
        char *parameter = macro.parameters[i];
        defineVariable(&macroArgsEnv, parameter, current->car);
        current = current->cdr;
    }

    struct Expression macroBody = deepCopy(&macro.body);
    struct Expression replacedBody = replaceMacroBody(env, &macroArgsEnv, macroBody);

    return replacedBody;
}

void includeFile(struct Environment *environment, char *path)
{
    char *fileContents = readFile(path);

    struct ExpressionLinkedList *expressions = sourceToExpressions(fileContents);
    struct Environment innerEnv = createBaseEnvironment();

    while (expressions != NULL)
    {
        evaluate(&innerEnv, expressions->value);
        expressions = expressions->next;
    }

    free(fileContents);

    for (size_t i = 0; i < HASH_SIZE; i++)
    {
        struct VariableNode *current = innerEnv.nodes[i];
        while (current != NULL)
        {
            defineVariable(environment, current->key, current->value);
            current = current->next;
        }
    }
}

struct ExpressionLinkedList *sourceToExpressions(char *source)
{
    struct ScannerStatus scannerStatus = parse(source);
    if (scannerStatus.hasError)
    {
        return NULL;
    }
    struct TokenLinkedList *tokens = scannerStatus.tokensHead.next;

    struct SyntaxAnalyserStatus syntaxStatus = syntaxAnalyser(tokens);
    if (syntaxStatus.hasError)
    {
        return NULL;
    }
    struct ExpressionLinkedList *expressions = syntaxStatus.expressions;

    while (tokens != NULL)
    {
        struct TokenLinkedList *current = tokens;
        tokens = tokens->next;
        free(current);
    }

    return expressions;
}
