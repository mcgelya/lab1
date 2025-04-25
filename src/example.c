#include "example.h"
#include "complex.h"
#include "integer.h"

bool isEven(const void *arg) {
    const Integer *val = (const Integer *) arg;
    return val->value % 2 == 0;
}

bool isZero(const void *arg) {
    const Integer *val = (const Integer *) arg;
    return val->value == 0;
}

bool isRe(const void *arg) {
    const Complex *val = (const Complex *) arg;
    return isZero(val->im);
}

bool isIm(const void *arg) {
    const Complex *val = (const Complex *) arg;
    return isZero(val->re);
}

void inc(void *dest, const void *source) {
    const Integer *val = (const Integer *) source;
    Integer x;
    x.value = val->value + 1;
    getIntegerTypeInfo()->assign(dest, &x);
}

void negate(void *dest, const void *source) {
    const Integer *val = (const Integer *) source;
    Integer x;
    x.value = -val->value;
    getIntegerTypeInfo()->assign(dest, &x);
}

void conjugate(void *dest, const void *source) {
    Complex *destComp = (Complex *) dest;
    const Complex *sourceComp = (const Complex *) source;
    getIntegerTypeInfo()->assign(destComp->re, sourceComp->re);
    negate(destComp->im, sourceComp->im);
}

void getRe(void *dest, const void *source) {
    Complex *destComp = (Complex *) dest;
    const Complex *sourceComp = (const Complex *) source;
    getIntegerTypeInfo()->assign(destComp->re, sourceComp->re);
    Integer x;
    x.value = 0;
    getIntegerTypeInfo()->assign(destComp->im, &x);
}

void getIm(void *dest, const void *source) {
    Complex *destComp = (Complex *) dest;
    const Complex *sourceComp = (const Complex *) source;
    Integer x;
    x.value = 0;
    getIntegerTypeInfo()->assign(destComp->re, &x);
    getIntegerTypeInfo()->assign(destComp->im, sourceComp->im);
}
