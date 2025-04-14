#include "complex.h"
#include "complex_on_integer.h"
#include "integer.h"
#include "vector.h"
#include <assert.h>
#include <stdio.h>

bool is_even(const void *arg) {
    const Integer *val = (const Integer *) arg;
    return val->value % 2 == 0;
}

void inc(void *dest, const void *source) {
    const Integer *val = (const Integer *) source;
    Integer x;
    x.value = val->value + 1;
    getIntegerTypeInfo()->assign(dest, &x);
}

int main() {
    int n;
    scanf("%d", &n);
    vector *v = emptyVector(getIntegerTypeInfo());
    for (size_t i = 0; i < n; ++i) {
        Integer x;
        scanf("%d", &x.value);
        push_back(v, &x);
    }
    print(v);
    vector *filtered = where(v, is_even);
    print(filtered);
    vector *mp = map(v, inc);
    print(mp);
    print(v);

    int m;
    scanf("%d", &m);
    vector *v_comp = emptyVector(getComplexOnIntegerTypeInfo());
    for (size_t i = 0; i < m; ++i) {
        Integer re, im;
        scanf("%d %d", &re.value, &im.value);
        Complex *z = getComplexOnIntegerTypeInfo()->alloc(1);
        getIntegerTypeInfo()->assign(z->re, &re);
        getIntegerTypeInfo()->assign(z->im, &im);
        push_back(v_comp, z);
    }
    print(v_comp);
    {
        pop_back(v_comp);
        Integer re, im;
        scanf("%d %d", &re.value, &im.value);
        Complex *z = getComplexOnIntegerTypeInfo()->alloc(1);
        getIntegerTypeInfo()->assign(z->re, &re);
        getIntegerTypeInfo()->assign(z->im, &im);
        push_back(v_comp, z);
    }
    print(v_comp);
}
