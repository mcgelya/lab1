#include "complex.h"

#include <stdio.h>

void assignComplex(void *destination, const void *source, const TypeInfo *argType) {
    Complex *destinationComp = (Complex *) destination;
    const Complex *sourceComp = (const Complex *) source;
    argType->assign(destinationComp->re, sourceComp->re);
    argType->assign(destinationComp->im, sourceComp->im);
}

size_t getSizeComplex() {
    return sizeof(Complex);
}

void allocComplexComponents(void *arg, const TypeInfo *argType) {
    Complex *argComp = (Complex *) arg;
    argComp->re = argType->alloc(1);
    argComp->im = argType->alloc(1);
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

bool eqOperatorComplex(const void *a, const void *b, const TypeInfo *argType) {
    const Complex *aComp = (const Complex *) a;
    const Complex *bComp = (const Complex *) b;
    return argType->eqOperator(aComp->re, bComp->re) && argType->eqOperator(aComp->im, bComp->im);
}

void printComplex(const void *arg, const TypeInfo *argType) {
    const Complex *argComp = (const Complex *) arg;
    argType->print(argComp->re);
    printf(" + i*");
    argType->print(argComp->im);
}
