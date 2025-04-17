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

void testNullArgType() {
    {
        ResultInfo *res = emptyVector(NULL);
        TEST(res->error->code == ERROR_NULL_POINTER);
    }
    {
        ResultInfo *res = nElementsVector(10, NULL);
        TEST(res->error->code == ERROR_NULL_POINTER);
    }
}

void testNullVector() {
    vector *v = (vector *) emptyVector(getIntegerTypeInfo())->result;
    Integer x;
    x.value = 1;
    {
        ErrorInfo *err = pushBack(NULL, &x);
        TEST(err->code == ERROR_NULL_POINTER);
    }
    {
        ErrorInfo *err = pushBack(v, NULL);
        TEST(err->code == ERROR_NULL_POINTER);
    }
    {
        ErrorInfo *err = popBack(NULL);
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
        pushBack(v, &x);
        ErrorInfo *err = assignElement(v, 0, NULL);
        TEST(err->code == ERROR_NULL_POINTER);
    }
    {
        ResultInfo *res = map(NULL, inc);
        TEST(res->error->code == ERROR_NULL_POINTER);
    }
    {
        ResultInfo *res = where(NULL, isEven);
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

void testOutOfBounds() {
    vector *v = (vector *) emptyVector(getIntegerTypeInfo())->result;
    Integer x;
    x.value = 1;
    {
        ErrorInfo *err = popBack(v);
        TEST(err->code == ERROR_OUT_OF_BOUNDS);
    }
    {
        pushBack(v, &x);
        ResultInfo *res = getElement(v, 1);
        TEST(res->error->code == ERROR_OUT_OF_BOUNDS);
    }
    {
        ErrorInfo *err = assignElement(v, 1, &x);
        TEST(err->code == ERROR_OUT_OF_BOUNDS);
    }
}

void testConcatTypeMismatch() {
    vector *a = (vector *) emptyVector(getIntegerTypeInfo())->result;
    vector *b = (vector *) emptyVector(getComplexOnIntegerTypeInfo())->result;
    ResultInfo *res = concatVectors(a, b);
    TEST(res->error->code == ERROR_TYPE_MISMATCH);
}

void testVector() {
    {
        Integer x;
        x.value = 1;
        vector *v = (vector *) emptyVector(getIntegerTypeInfo())->result;
        pushBack(v, &x); // 1
        x.value++;
        pushBack(v, &x); // 1 2
        x.value++;
        pushBack(v, &x); // 1 2 3
        x.value++;
        pushBack(v, &x); // 1 2 3 4
        x.value++;
        pushBack(v, &x); // 1 2 3 4 5
        popBack(v); // 1 2 3 4
        popBack(v); // 1 2 3
        pushBack(v, &x); // 1 2 3 5

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
            pushBack(v, &x);
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
        pushBack(v, z); // 1 + 1i
        ((Integer *) z->re)->value++;
        pushBack(v, z); // 1 + 1i, 2 + 1i
        ((Integer *) z->im)->value++;
        pushBack(v, z); // 1 + 1i, 2 + 1i, 2 + 2i
        popBack(v); // 1 + 1i, 2 + 1i

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

void testMap() {
    {
        size_t n = 10;
        vector *v = (vector *) nElementsVector(n, getIntegerTypeInfo())->result;
        for (size_t i = 0; i < n; ++i) {
            Integer x;
            x.value = rand() % 100;
            assignElement(v, i, &x);
        }
        vector *mp = (vector *) map(v, inc)->result;
        bool checkMap = 1;
        for (size_t i = 0; i < n; ++i) {
            Integer x;
            inc(&x, getElement(v, i)->result);
            if (!getIntegerTypeInfo()->eqOperator(getElement(mp, i)->result, &x)) {
                checkMap = 0;
                break;
            }
        }
        TEST(checkMap);
    }
    {
        size_t n = 10;
        vector *v = (vector *) nElementsVector(n, getComplexOnIntegerTypeInfo())->result;
        for (size_t i = 0; i < n; ++i) {
            Integer re;
            re.value = rand() % 100;
            Integer im;
            im.value = rand() % 100;
            Complex *z = getComplexOnIntegerTypeInfo()->alloc(1);
            getIntegerTypeInfo()->assign(z->re, &re);
            getIntegerTypeInfo()->assign(z->im, &im);
            assignElement(v, i, z);
        }
        vector *mp = (vector *) map(v, conjugate)->result;
        bool checkMap = 1;
        for (size_t i = 0; i < n; ++i) {
            Complex *z = getComplexOnIntegerTypeInfo()->alloc(1);
            conjugate(z, getElement(v, i)->result);
            if (!getComplexOnIntegerTypeInfo()->eqOperator(getElement(mp, i)->result, z)) {
                checkMap = 0;
                break;
            }
        }
        TEST(checkMap);
    }
}

void checkVectorsEqual(const vector *a, const vector *b) {
    TEST(a->size == b->size);
    TEST(a->argType == b->argType);
    bool checkWhere = 1;
    for (size_t i = 0; i < a->size; ++i) {
        if (!a->argType->eqOperator(getElement(a, i)->result, getElement(b, i)->result)) {
            checkWhere = 0;
            break;
        }
    }
    TEST(checkWhere);
}

void testWhere() {
    {
        size_t n = 10;
        vector *a = (vector *) nElementsVector(n, getIntegerTypeInfo())->result;
        vector *c = (vector *) emptyVector(getIntegerTypeInfo())->result;
        for (size_t i = 0; i < n; ++i) {
            Integer x;
            x.value = rand() % 100;
            if (isEven(&x)) {
                pushBack(c, &x);
            }
            assignElement(a, i, &x);
        }
        vector *filtered = (vector *) where(a, isEven)->result;
        checkVectorsEqual(c, filtered);
    }
    {
        size_t n = 10;
        vector *a = (vector *) nElementsVector(n, getComplexOnIntegerTypeInfo())->result;
        vector *c = (vector *) emptyVector(getComplexOnIntegerTypeInfo())->result;
        for (size_t i = 0; i < n; ++i) {
            Integer re;
            re.value = rand() % 4;
            Integer im;
            im.value = rand() % 4;
            Complex *z = getComplexOnIntegerTypeInfo()->alloc(1);
            getIntegerTypeInfo()->assign(z->re, &re);
            getIntegerTypeInfo()->assign(z->im, &im);
            if (isRe(z)) {
                pushBack(c, z);
            }
            assignElement(a, i, z);
        }
        vector *filtered = (vector *) where(a, isRe)->result;
        checkVectorsEqual(c, filtered);
    }
}

void checkConcatVectors(const vector *a, const vector *b, const vector *c) {
    TEST(a->argType == c->argType && b->argType == c->argType);
    TEST(a->size + b->size == c->size);
    bool checkConcat = 1;
    for (int i = 0; i < a->size; ++i) {
        if (!a->argType->eqOperator(getElement(a, i)->result, getElement(c, i)->result)) {
            checkConcat = 0;
            break;
        }
    }
    for (int i = 0; i < b->size; ++i) {
        if (!a->argType->eqOperator(getElement(b, i)->result, getElement(c, a->size + i)->result)) {
            checkConcat = 0;
            break;
        }
    }
    TEST(checkConcat);
}

void testConcat() {
    {
        int n = 10;
        vector *a = (vector *) nElementsVector(n, getIntegerTypeInfo())->result;
        for (int i = 0; i < n; ++i) {
            Integer x;
            x.value = rand() % 100;
            assignElement(a, i, &x);
        }
        int m = 15;
        vector *b = (vector *) nElementsVector(m, getIntegerTypeInfo())->result;
        for (int i = 0; i < m; ++i) {
            Integer x;
            x.value = rand() % 100;
            assignElement(b, i, &x);
        }
        vector *c = (vector *) concatVectors(a, b)->result;
        checkConcatVectors(a, b, c);
    }
    {
        int n = 10;
        vector *a = (vector *) nElementsVector(n, getIntegerTypeInfo())->result;
        for (int i = 0; i < n; ++i) {
            Integer x;
            x.value = rand() % 100;
            assignElement(a, i, &x);
        }
        vector *c = (vector *) concatVectors(a, a)->result;
        checkConcatVectors(a, a, c);
    }
    {
        int n = 10;
        vector *a = (vector *) nElementsVector(n, getComplexOnIntegerTypeInfo())->result;
        for (int i = 0; i < n; ++i) {
            Integer re;
            re.value = rand() % 100;
            Integer im;
            im.value = rand() % 100;
            Complex *z = getComplexOnIntegerTypeInfo()->alloc(1);
            getIntegerTypeInfo()->assign(z->re, &re);
            getIntegerTypeInfo()->assign(z->im, &im);
            assignElement(a, i, z);
        }
        int m = 15;
        vector *b = (vector *) nElementsVector(m, getComplexOnIntegerTypeInfo())->result;
        for (int i = 0; i < m; ++i) {
            Integer re;
            re.value = rand() % 100;
            Integer im;
            im.value = rand() % 100;
            Complex *z = getComplexOnIntegerTypeInfo()->alloc(1);
            getIntegerTypeInfo()->assign(z->re, &re);
            getIntegerTypeInfo()->assign(z->im, &im);
            assignElement(b, i, z);
        }
        vector *c = (vector *) concatVectors(a, b)->result;
        checkConcatVectors(a, b, c);
    }
    {
        int n = 10;
        vector *a = (vector *) nElementsVector(n, getComplexOnIntegerTypeInfo())->result;
        for (int i = 0; i < n; ++i) {
            Integer re;
            re.value = rand() % 100;
            Integer im;
            im.value = rand() % 100;
            Complex *z = getComplexOnIntegerTypeInfo()->alloc(1);
            getIntegerTypeInfo()->assign(z->re, &re);
            getIntegerTypeInfo()->assign(z->im, &im);
            assignElement(a, i, z);
        }
        vector *c = (vector *) concatVectors(a, a)->result;
        checkConcatVectors(a, a, c);
    }
}

int main() {
    testNullArgType();
    testNullVector();

    testOutOfBounds();
    testConcatTypeMismatch();

    testVector();

    testMap();
    testWhere();
    testConcat();

    printf("\nOK tests: %d\n", okTests);
    printf("Failed tests: %d\n", failedTests);
}
