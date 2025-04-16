#include "../src/complex.h"
#include "../src/complex_on_integer.h"
#include "../src/example.h"
#include "../src/integer.h"
#include "../src/vector.h"

#include <stdio.h>
#include <stdlib.h>

int failedTests = 0;
int okTests = 0;

#define TEST(cond)                                                                                  \
    do {                                                                                            \
        if (!(cond)) {                                                                              \
            printf("Assertion failed: %s\nIn function: %s (line %d)\n", #cond, __func__, __LINE__); \
            ++failedTests;                                                                          \
        } else {                                                                                    \
            printf("%s is OK.\n", #cond);                                                           \
            ++okTests;                                                                              \
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

void test_null_vector() {
    vector *v = (vector *) emptyVector(getIntegerTypeInfo())->result;
    Integer x;
    x.value = 1;
    {
        ErrorInfo *err = push_back(NULL, &x);
        TEST(err->code == ERROR_NULL_POINTER);
    }
    {
        ErrorInfo *err = push_back(v, NULL);
        TEST(err->code == ERROR_NULL_POINTER);
    }
    {
        ErrorInfo *err = pop_back(NULL);
        TEST(err->code == ERROR_NULL_POINTER);
    }
    {
        ResultInfo *res = getElement(NULL, 0);
        TEST(res->error->code == ERROR_NULL_POINTER);
    }
    {
        ErrorInfo *err = assignElement(NULL, 0, &x);
        TEST(err->code == ERROR_NULL_POINTER);
    }
    {
        ErrorInfo *err = assignElement(NULL, 0, &x);
        TEST(err->code == ERROR_NULL_POINTER);
    }
    {
        push_back(v, &x);
        ErrorInfo *err = assignElement(v, 0, NULL);
        TEST(err->code == ERROR_NULL_POINTER);
    }
    {
        ResultInfo *res = map(NULL, inc);
        TEST(res->error->code == ERROR_NULL_POINTER);
    }
    {
        ResultInfo *res = where(NULL, is_even);
        TEST(res->error->code == ERROR_NULL_POINTER);
    }
    {
        ResultInfo *res = concatVectors(NULL, v);
        TEST(res->error->code == ERROR_NULL_POINTER);
    }
    {
        ResultInfo *res = concatVectors(v, NULL);
        TEST(res->error->code == ERROR_NULL_POINTER);
    }
    {
        ResultInfo *res = concatVectors(NULL, NULL);
        TEST(res->error->code == ERROR_NULL_POINTER);
    }
    {
        ErrorInfo *err = print(NULL);
        TEST(err->code == ERROR_NULL_POINTER);
    }
    {
        ResultInfo *res = map(v, NULL);
        TEST(res->error->code == ERROR_NULL_POINTER);
    }
    {
        ResultInfo *res = where(v, NULL);
        TEST(res->error->code == ERROR_NULL_POINTER);
    }
}

void test_out_of_bounds() {
    vector *v = (vector *) emptyVector(getIntegerTypeInfo())->result;
    Integer x;
    x.value = 1;
    {
        ErrorInfo *err = pop_back(v);
        TEST(err->code == ERROR_OUT_OF_BOUNDS);
    }
    {
        push_back(v, &x);
        ResultInfo *res = getElement(v, 1);
        TEST(res->error->code == ERROR_OUT_OF_BOUNDS);
    }
    {
        ErrorInfo *err = assignElement(v, 1, &x);
        TEST(err->code == ERROR_OUT_OF_BOUNDS);
    }
}

void test_concat_type_mismatch() {
    vector *a = (vector *) emptyVector(getIntegerTypeInfo())->result;
    vector *b = (vector *) emptyVector(getComplexOnIntegerTypeInfo())->result;
    ResultInfo *res = concatVectors(a, b);
    TEST(res->error->code == ERROR_TYPE_MISMATCH);
}

void test_vector() {
    {
        Integer x;
        x.value = 1;
        vector *v = (vector *) emptyVector(getIntegerTypeInfo())->result;
        push_back(v, &x); // 1
        x.value++;
        push_back(v, &x); // 1 2
        x.value++;
        push_back(v, &x); // 1 2 3
        x.value++;
        push_back(v, &x); // 1 2 3 4
        x.value++;
        push_back(v, &x); // 1 2 3 4 5
        pop_back(v); // 1 2 3 4
        pop_back(v); // 1 2 3
        push_back(v, &x); // 1 2 3 5

        TEST(v->size == 4);
        TEST(((Integer *) getElement(v, 0)->result)->value == 1);
        TEST(((Integer *) getElement(v, 1)->result)->value == 2);
        TEST(((Integer *) getElement(v, 2)->result)->value == 3);
        TEST(((Integer *) getElement(v, 3)->result)->value == 5);

        assignElement(v, 0, &x); // 5 2 3 5
        x.value++;
        assignElement(v, 1, &x); // 5 6 3 5

        TEST(v->size == 4);
        TEST(((Integer *) getElement(v, 0)->result)->value == 5);
        TEST(((Integer *) getElement(v, 1)->result)->value == 6);
        TEST(((Integer *) getElement(v, 2)->result)->value == 3);
        TEST(((Integer *) getElement(v, 3)->result)->value == 5);
    }
    {
        int n = 1000;
        vector *v = (vector *) emptyVector(getIntegerTypeInfo())->result;
        for (int i = 0; i < n; ++i) {
            Integer x;
            x.value = i;
            push_back(v, &x);
        }
        TEST(v->size == n);
        bool testLargeVector = 1;
        for (int i = 0; i < n; ++i) {
            if (((Integer *) getElement(v, i)->result)->value != i) {
                testLargeVector = 0;
                break;
            }
        }
        TEST(testLargeVector);
    }
    {
        Integer x;
        x.value = 1;
        Complex *z = getComplexOnIntegerTypeInfo()->alloc(1);
        getIntegerTypeInfo()->assign(z->re, &x);
        getIntegerTypeInfo()->assign(z->im, &x);
        vector *v = (vector *) emptyVector(getComplexOnIntegerTypeInfo())->result;
        push_back(v, z); // 1 + 1i
        ((Integer *) z->re)->value++;
        push_back(v, z); // 1 + 1i, 2 + 1i
        ((Integer *) z->im)->value++;
        push_back(v, z); // 1 + 1i, 2 + 1i, 2 + 2i
        pop_back(v); // 1 + 1i, 2 + 1i

        TEST(v->size == 2);
        Complex *z1 = getComplexOnIntegerTypeInfo()->alloc(1);
        getIntegerTypeInfo()->assign(z1->re, &x);
        getIntegerTypeInfo()->assign(z1->im, &x);
        TEST(getComplexOnIntegerTypeInfo()->eqOperator(getElement(v, 0)->result, z1));
        ((Integer *) z1->re)->value++;
        TEST(getComplexOnIntegerTypeInfo()->eqOperator(getElement(v, 1)->result, z1));

        assignElement(v, 0, z); // 2 + 2i, 2 + 1i

        TEST(v->size == 2);
        TEST(getComplexOnIntegerTypeInfo()->eqOperator(getElement(v, 0)->result, z));
        TEST(getComplexOnIntegerTypeInfo()->eqOperator(getElement(v, 1)->result, z1));
    }
}

void test_map() {
    {
        int n = 10;
        vector *v = (vector *) nElementsVector(n, getIntegerTypeInfo())->result;
        for (int i = 0; i < n; ++i) {
            Integer x;
            x.value = rand() % 100;
            assignElement(v, i, &x);
        }
        vector *mp = map(v, inc)->result;
        bool testMap = 1;
        for (int i = 0; i < n; ++i) {
            Integer x;
            inc(&x, getElement(v, i)->result);
            if (!getIntegerTypeInfo()->eqOperator(getElement(mp, i)->result, &x)) {
                testMap = 0;
                break;
            }
        }
        TEST(testMap);
    }
    {
        int n = 10;
        vector *v = (vector *) nElementsVector(n, getComplexOnIntegerTypeInfo())->result;
        for (int i = 0; i < n; ++i) {
            Integer re;
            re.value = rand() % 100;
            Integer im;
            im.value = rand() % 100;
            Complex *z = getComplexOnIntegerTypeInfo()->alloc(1);
            getIntegerTypeInfo()->assign(z->re, &re);
            getIntegerTypeInfo()->assign(z->im, &im);
            assignElement(v, i, z);
        }
        vector *mp = map(v, conjugate)->result;
        bool testMap = 1;
        for (int i = 0; i < n; ++i) {
            Complex *z = getComplexOnIntegerTypeInfo()->alloc(1);
            conjugate(z, getElement(v, i)->result);
            if (!getComplexOnIntegerTypeInfo()->eqOperator(getElement(mp, i)->result, z)) {
                testMap = 0;
                break;
            }
        }
        TEST(testMap);
    }
}

void test_where() {
}

void test_concat() {
}

int main() {
    test_null_argType();
    test_null_vector();

    test_out_of_bounds();
    test_concat_type_mismatch();

    test_vector();

    test_map();
    test_where();
    test_concat();

    printf("\nOK tests: %d\n", okTests);
    printf("Failed tests: %d\n", failedTests);
}
