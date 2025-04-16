#include "example.h"
#include "complex.h"
#include "integer.h"

bool is_even(const void *arg) {
    const Integer *val = (const Integer *) arg;
    return val->value % 2 == 0;
}

bool is_zero(const void *arg) {
    const Integer *val = (const Integer *) arg;
    return val->value == 0;
}

bool is_re(const void *arg) {
    const Complex *val = (const Complex *) arg;
    return is_zero(val->im);
}

bool is_im(const void *arg) {
    const Complex *val = (const Complex *) arg;
    return is_zero(val->re);
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
