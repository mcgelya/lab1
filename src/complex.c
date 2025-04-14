#include "complex.h"

#include <assert.h>
#include <stdio.h>

void assignComplex(void *destination, const void *source, const TypeInfo *argType) {
    Complex *destination_comp = (Complex *) destination;
    const Complex *source_comp = (const Complex *) source;
    argType->assign(destination_comp->re, source_comp->re);
    argType->assign(destination_comp->im, source_comp->im);
}

size_t getSizeComplex() {
    return sizeof(Complex);
}

void allocComplexComponents(void *arg, const TypeInfo *argType) {
    Complex *arg_comp = (Complex *) arg;
    arg_comp->re = argType->alloc(1);
    arg_comp->im = argType->alloc(1);
}

void *allocComplex(size_t n, const TypeInfo *argType) {
    size_t width = getSizeComplex();
    void *comp = malloc(width * n);
    char *ptr = (char *) comp;
    for (size_t i = 0; i < n; ++i) {
        allocComplexComponents((void *) (ptr + width * i), argType);
    }
    return comp;
}

bool eqOperComplex(const void *a, const void *b, const TypeInfo *argType) {
    const Complex *a_comp = (const Complex *) a;
    const Complex *b_comp = (const Complex *) b;
    return argType->eqOperator(a_comp->re, b_comp->re) && argType->eqOperator(a_comp->im, b_comp->im);
}

void printComplex(const void *arg, const TypeInfo *argType) {
    const Complex *arg_comp = (const Complex *) arg;
    argType->print(arg_comp->re);
    printf(" + i*");
    argType->print(arg_comp->im);
}
