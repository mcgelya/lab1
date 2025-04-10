#pragma once

#include "type_info.h"

typedef struct Complex {
    void *re;
    void *im;

    const TypeInfo *argType;
} Complex;

void assignComplex(void *destination, const void *source);
size_t getSizeComplex(const TypeInfo *argType);
void *allocComplex(const TypeInfo *argType);
void printComplex(const void *arg);
