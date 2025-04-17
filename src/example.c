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
    Complex *dest_comp = (Complex *) dest;
    const Complex *source_comp = (const Complex *) source;
    getIntegerTypeInfo()->assign(dest_comp->re, source_comp->re);
    negate(dest_comp->im, source_comp->im);
}
