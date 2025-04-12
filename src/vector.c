#include "vector.h"
#include <assert.h>
#include <stdio.h>

vector *emptyVector(const TypeInfo *argType) {
    vector *res = malloc(sizeof(vector));
    res->start = argType->alloc(1);
    res->size = 0;
    res->capacity = 1;
    res->argType = argType;
    return res;
}

vector *nElementsVector(size_t n, const TypeInfo *argType) {
    vector *res = malloc(sizeof(vector));
    res->start = argType->alloc(n);
    res->size = n;
    res->capacity = n;
    res->argType = argType;
    return res;
}

void push_back(vector *v, const void *source) {
    size_t width = v->argType->getSize();
    if (v->size < v->capacity) {
        char *ptr = (char *) v->start;
        v->argType->assign((void *) (ptr + width * v->size), source);
        ++v->size;
    } else {
        assert(v->size == v->capacity);
        v->capacity *= 2;
        void *new_start = v->argType->alloc(v->capacity);
        char *ptr = (char *) v->start;
        char *dest_ptr = (char *) new_start;
        for (size_t i = 0; i < v->size; ++i) {
            v->argType->assign((void *) (dest_ptr + width * i), (void *) (ptr + width * i));
        }
        v->argType->assign((void *) (dest_ptr + width * v->size), source);
        free(v->start);
        v->start = new_start;
        ++v->size;
    }
}

void pop_back(vector *v) {
    assert(v->size > 0);
    --v->size;
}

const void *getElement(vector *v, size_t i) {
    assert(i < v->size);
    size_t width = v->argType->getSize();
    char *ptr = (char *) v->start;
    return (void *) (ptr + width * i);
}

void assignElement(vector *v, size_t i, const void *source) {
    assert(i < v->size);
    size_t width = v->argType->getSize();
    char *ptr = (char *) v->start;
    v->argType->assign((void *) (ptr + width * i), source);
}

size_t getLength(vector *v) {
    return v->size;
}

vector *map(const vector *v, UnaryOperator func) {
    vector *res = nElementsVector(v->size, v->argType);
    size_t width = v->argType->getSize();
    char *ptr = (char *) v->start;
    char *res_ptr = (char *) res->start;
    for (size_t i = 0; i < v->size; ++i) {
        func((void *) (res_ptr + width * i), (const void *) (ptr + width * i));
    }
    return res;
}

vector *where(const vector *v, Predicate pred) {
    vector *res = emptyVector(v->argType);
    size_t width = v->argType->getSize();
    char *ptr = (char *) v->start;
    for (size_t i = 0; i < v->size; ++i) {
        const void *cur = (const void *) (ptr + width * i);
        if (pred(cur)) {
            push_back(res, cur);
        }
    }
    return res;
}

vector *concatVectors(const vector *first, const vector *second) {
    assert(first->argType == second->argType);
    vector *res = emptyVector(first->argType);
    size_t width = first->argType->getSize();
    char *ptr_a = (char *) first->start;
    for (size_t i = 0; i < first->size; ++i) {
        const void *cur = (const void *) (ptr_a + width * i);
        push_back(res, cur);
    }
    char *ptr_b = (char *) second->start;
    for (size_t i = 0; i < second->size; ++i) {
        const void *cur = (const void *) (ptr_b + width * i);
        push_back(res, cur);
    }
    return res;
}

void print(const vector *v) {
    size_t width = v->argType->getSize();
    char *ptr = (char *) v->start;
    for (size_t i = 0; i < v->size; ++i) {
        v->argType->print((const void *) (ptr + width * i));
        if (i + 1 < v->size) {
            printf(", ");
        }
    }
    printf("\n");
}
