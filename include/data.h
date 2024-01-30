#ifndef DATA_H
#define DATA_H

/*
- integer, float (number)
- string X
- cons
- symbol
- vector
- hash-table
- subr
- byte-code function
- record
*/

typedef enum DataType
{
    NUMBER,
    STRING
} DataType;

typedef union Value
{
    double number;
    char *string;
} Value;

typedef struct Data
{
    DataType type;
    Value value;
} Data;

#endif
