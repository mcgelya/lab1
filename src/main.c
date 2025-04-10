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
}
