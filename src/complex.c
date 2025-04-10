#include "complex.h"

#include <assert.h>
#include <stdio.h>

void assignComplex(void *destination, const void *source) {
    Complex *destination_comp = (Complex *) destination;
    const Complex *source_comp = (const Complex *) source;

    assert(destination_comp->argType == source_comp->argType);
    const TypeInfo *type = destination_comp->argType;
    type->assign(destination_comp->re, source_comp->re);
    type->assign(destination_comp->im, source_comp->im);
}

size_t getSizeComplex(const TypeInfo *argType) {
    return argType->getSize() * 2;
}

void *allocComplex(const TypeInfo *argType) {
    return malloc(getSizeComplex(argType));
}

void printComplex(const void *arg) {
    const Complex *arg_comp = (const Complex *) arg;
    arg_comp->argType->print(arg_comp->re);
    printf(" + i*");
    arg_comp->argType->print(arg_comp->im);
}
