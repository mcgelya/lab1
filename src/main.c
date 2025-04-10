#include "integer.h"
#include "vector.h"
#include <assert.h>
#include <stdio.h>

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
    pop_back(v);
    print(v);
}
