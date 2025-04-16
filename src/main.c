#include "complex.h"
#include "complex_on_integer.h"
#include "integer.h"
#include "vector.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

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

int main(int argc, char *argv[]) {
    if (argc == 1 || strcmp("int", argv[1]) == 0) {
        vector *v = (vector *) emptyVector(getIntegerTypeInfo())->result;
        printf("Let's play. Enter '+ x' to push_back x, '-' to pop_back, or '!' to quit:\n");
        while (true) {
            char c;
            scanf("%c", &c);
            if (c == '+') {
                Integer x;
                scanf("%d", &x.value);
                push_back(v, &x);
            } else if (c == '-') {
                ErrorInfo *err = pop_back(v);
                if (err != getSuccessResult()) {
                    printf("%s\n", err->msg);
                    return 0;
                }
            } else if (c == '!') {
                break;
            }
        }
        printf("\nGreat! Here's your vector:\n");
        print(v);

        vector *filtered = (vector *) where(v, is_even)->result;
        printf("\nFiltered to leave only even numbers:\n");
        print(filtered);

        vector *mp = (vector *) map(v, inc)->result;
        printf("\nMapped with 'inc', all values increased by one:\n");
        print(mp);

        vector *conc = (vector *) concatVectors(filtered, mp)->result;
        printf("\nConcatenated these vectors:\n");
        print(conc);
    } else {
        vector *v = (vector *) emptyVector(getComplexOnIntegerTypeInfo())->result;
        printf("Let's play. Enter '+ re im' to push_back re + i * im, '-' to pop_back, or '!' to quit:\n");
        while (true) {
            char c;
            scanf("%c", &c);
            if (c == '+') {
                Integer re, im;
                scanf("%d %d", &re.value, &im.value);
                Complex *z = getComplexOnIntegerTypeInfo()->alloc(1);
                getIntegerTypeInfo()->assign(z->re, &re);
                getIntegerTypeInfo()->assign(z->im, &im);
                push_back(v, z);
            } else if (c == '-') {
                ErrorInfo *err = pop_back(v);
                if (err != getSuccessResult()) {
                    printf("%s\n", err->msg);
                    return 0;
                }
            } else if (c == '!') {
                break;
            }
        }
        printf("\nGreat! Here's your vector:\n");
        print(v);

        vector *filtered = (vector *) where(v, is_re)->result;
        printf("\nFiltered to leave only real numbers:\n");
        print(filtered);

        printf("\nMapped with 'conjugate':\n");
        vector *mp = (vector *) map(v, conjugate)->result;
        print(mp);

        vector *conc = (vector *) concatVectors(filtered, mp)->result;
        printf("\nConcatenated these vectors:\n");
        print(conc);
    }
}
