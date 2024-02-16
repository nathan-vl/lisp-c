#include <stdio.h>
#include <stdlib.h>

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

struct Expression replaceMacroBody(struct Environment *env, struct Environment *macroArgs, struct Macro macro)
{
    if (macro.body->kind == SYMBOL)
    {
        struct Expression *macroArg = getVariable(macroArgs, macro.body->value.symbol);
        if (macroArg != NULL)
        {
            return *macroArg;
        }
    }

    struct Expression body = evaluate(env, *macro.body);
    if (body.kind != LIST)
    {
        return body;
    }

    struct List *current = body.value.list;

    while (current != NULL)
    {
        struct Macro inner = macro;
        *inner.body = current->car;
        current->car = replaceMacroBody(env, macroArgs, inner);
        current = current->cdr;
    }

    return body;
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

    struct Expression replacedBody = replaceMacroBody(env, &macroArgsEnv, macro);
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
