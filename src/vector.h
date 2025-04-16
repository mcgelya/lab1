#pragma once

#include "error_info.h"
#include "type_info.h"
#include <stdbool.h>

typedef struct vector {
    void *start;
    size_t size;
    size_t capacity;

    const TypeInfo *argType;
} vector;

ResultInfo *emptyVector(const TypeInfo *argType);
ResultInfo *nElementsVector(size_t n, const TypeInfo *argType);

ErrorInfo *push_back(vector *v, const void *source);
ErrorInfo *pop_back(vector *v);

ResultInfo *getElement(vector *v, size_t i);

ErrorInfo *assignElement(vector *v, size_t i, const void *source);

ResultInfo *map(const vector *v, UnaryOperator func);

typedef bool (*Predicate)(const void *arg);
ResultInfo *where(const vector *v, Predicate pred);

ResultInfo *concatVectors(const vector *first, const vector *second);

ErrorInfo *print(const vector *v);
