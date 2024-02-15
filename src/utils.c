#include <stdio.h>
#include <stdlib.h>

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
