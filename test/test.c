#include "../src/complex.h"
#include "../src/complex_on_integer.h"
#include "../src/integer.h"
#include "../src/vector.h"

#include <stdio.h>
#include <stdlib.h>


#define TEST(cond)                                                                                  \
    do {                                                                                            \
        if (!(cond)) {                                                                              \
            printf("Assertion failed: %s\nIn function: %s (line %d)\n", #cond, __func__, __LINE__); \
            return;                                                                                 \
        } else {                                                                                    \
            printf("%s is OK.\n", #cond);                                                           \
        }                                                                                           \
    } while (0)

void test_null_argType() {
    {
        ResultInfo *res = emptyVector(NULL);
        TEST(res->error->code == ERROR_NULL_POINTER);
    }
    {
        ResultInfo *res = nElementsVector(10, NULL);
        TEST(res->error->code == ERROR_NULL_POINTER);
    }
}

void test_null_push_back() {
    Integer x;
    x.value = 1;
    vector *v = (vector *) emptyVector(getIntegerTypeInfo())->result;
    {
        ErrorInfo *err = push_back(NULL, &x);
        TEST(err->code == ERROR_NULL_POINTER);
    }
    {
        ErrorInfo *err = push_back(v, NULL);
        TEST(err->code == ERROR_NULL_POINTER);
    }
}

void test_null_pop_back() {
    ErrorInfo *err = pop_back(NULL);
    TEST(err->code == ERROR_NULL_POINTER);
}

void test_pop_back_empty() {
    vector *v = (vector *) emptyVector(getIntegerTypeInfo())->result;
    ErrorInfo *err = pop_back(v);
    TEST(err->code == ERROR_OUT_OF_BOUNDS);
}

int main() {
    test_null_argType();
    test_null_pop_back();
    test_null_push_back();
    test_pop_back_empty();
}
