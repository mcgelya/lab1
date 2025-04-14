#pragma once

#include "type_info.h"

typedef struct Complex {
    void *re;
    void *im;
} Complex;

void assignComplex(void *destination, const void *source, const TypeInfo *argType);
size_t getSizeComplex();
void allocComplexComponents(void *arg, const TypeInfo *argType);
void *allocComplex(size_t n, const TypeInfo *argType);
bool eqOperComplex(const void *a, const void *b, const TypeInfo *argType);
void printComplex(const void *arg, const TypeInfo *argType);
