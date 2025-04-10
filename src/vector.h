#pragma once

#include "type_info.h"
#include <stdbool.h>

typedef struct vector {
    void *start;
    size_t size;
    size_t capacity;

    const TypeInfo *argType;
} vector;

vector *emptyVector(const TypeInfo *argType);
vector *nElementsVector(size_t n, const TypeInfo *argType);

void push_back(vector *v, const void *source);
void pop_back(vector *v);

const void *getElement(vector *v, size_t i);

void assignElement(vector *v, size_t i, const void *source);

size_t getLength(vector *v);

vector *map(const vector *v, UnaryOperator func);

typedef bool (*Predicate)(const void *arg);
vector *where(const vector *v, Predicate pred);

vector *concatVectors(const vector *first, const vector *second);

void print(const vector *v);
