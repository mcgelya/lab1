#include "vector.h"
#include <assert.h>
#include <stdio.h>

ResultInfo *nElementsVector(size_t n, const TypeInfo *argType) {
    ResultInfo *res = malloc(sizeof(ResultInfo));
    if (argType == NULL) {
        res->result = NULL;
        res->error = getErrorInfo(ERROR_NULL_POINTER, "Error: Null pointer to argType.");
        return res;
    }
    vector *vec = malloc(sizeof(vector));
    vec->size = n;
    if (n == 0) {
        vec->capacity = 1;
    } else {
        vec->capacity = n;
    }
    vec->start = argType->alloc(vec->capacity);
    vec->argType = argType;
    res->result = (void *) vec;
    res->error = getSuccessInfo();
    return res;
}

ResultInfo *emptyVector(const TypeInfo *argType) {
    return nElementsVector(0, argType);
}

const ErrorInfo *pushBack(vector *v, const void *source) {
    if (v == NULL) {
        return getErrorInfo(ERROR_NULL_POINTER, "Error: Null pointer to vector.");
    }
    if (source == NULL) {
        return getErrorInfo(ERROR_NULL_POINTER, "Error: Null pointer to element.");
    }
    size_t width = v->argType->getSize();
    if (v->size < v->capacity) {
        char *ptr = (char *) v->start;
        v->argType->assign((void *) (ptr + width * v->size), source);
        ++v->size;
    } else {
        assert(v->size == v->capacity);
        v->capacity *= 2;
        void *newStart = v->argType->alloc(v->capacity);
        char *ptr = (char *) v->start;
        char *destPtr = (char *) newStart;
        for (size_t i = 0; i < v->size; ++i) {
            v->argType->assign((void *) (destPtr + width * i), (void *) (ptr + width * i));
        }
        v->argType->assign((void *) (destPtr + width * v->size), source);
        free(v->start);
        v->start = newStart;
        ++v->size;
    }
    return getSuccessInfo();
}

const ErrorInfo *popBack(vector *v) {
    if (v == NULL) {
        return getErrorInfo(ERROR_NULL_POINTER, "Error: Null pointer to vector.");
    }
    if (v->size == 0) {
        return getErrorInfo(ERROR_OUT_OF_BOUNDS, "Error: Cannot pop element from an empty vector.");
    }
    --v->size;
    return getSuccessInfo();
}

ResultInfo *getElement(const vector *v, size_t i) {
    ResultInfo *res = malloc(sizeof(ResultInfo));
    if (v == NULL) {
        res->result = NULL;
        res->error = getErrorInfo(ERROR_NULL_POINTER, "Error: Null pointer to vector.");
        return res;
    }
    if (i >= v->size) {
        res->result = NULL;
        res->error = getErrorInfo(ERROR_OUT_OF_BOUNDS, "Error: index is larger than size.");
        return res;
    }
    size_t width = v->argType->getSize();
    char *ptr = (char *) v->start;
    res->result = (void *) (ptr + width * i);
    res->error = getSuccessInfo();
    return res;
}

const ErrorInfo *assignElement(vector *v, size_t i, const void *source) {
    if (v == NULL) {
        return getErrorInfo(ERROR_NULL_POINTER, "Error: Null pointer to vector.");
    }
    if (source == NULL) {
        return getErrorInfo(ERROR_NULL_POINTER, "Error: Null pointer to element.");
    }
    if (i >= v->size) {
        return getErrorInfo(ERROR_OUT_OF_BOUNDS, "Error: index is larger than size.");
    }
    size_t width = v->argType->getSize();
    char *ptr = (char *) v->start;
    v->argType->assign((void *) (ptr + width * i), source);
    return getSuccessInfo();
}

ResultInfo *map(const vector *v, UnaryOperator func) {
    ResultInfo *res = malloc(sizeof(ResultInfo));
    if (v == NULL) {
        res->result = NULL;
        res->error = getErrorInfo(ERROR_NULL_POINTER, "Error: Null pointer to vector.");
        return res;
    }
    if (func == NULL) {
        res->result = NULL;
        res->error = getErrorInfo(ERROR_NULL_POINTER, "Error: Null pointer to function.");
        return res;
    }
    vector *vec = (vector *) nElementsVector(v->size, v->argType)->result;
    size_t width = v->argType->getSize();
    char *ptr = (char *) v->start;
    char *res_ptr = (char *) vec->start;
    for (size_t i = 0; i < v->size; ++i) {
        func((void *) (res_ptr + width * i), (const void *) (ptr + width * i));
    }
    res->result = (void *) vec;
    res->error = getSuccessInfo();
    return res;
}

ResultInfo *where(const vector *v, Predicate pred) {
    ResultInfo *res = malloc(sizeof(ResultInfo));
    if (v == NULL) {
        res->result = NULL;
        res->error = getErrorInfo(ERROR_NULL_POINTER, "Error: Null pointer to vector.");
        return res;
    }
    if (pred == NULL) {
        res->result = NULL;
        res->error = getErrorInfo(ERROR_NULL_POINTER, "Error: Null pointer to function.");
        return res;
    }
    vector *vec = (vector *) emptyVector(v->argType)->result;
    size_t width = v->argType->getSize();
    char *ptr = (char *) v->start;
    for (size_t i = 0; i < v->size; ++i) {
        const void *cur = (const void *) (ptr + width * i);
        if (pred(cur)) {
            pushBack(vec, cur);
        }
    }
    res->result = (void *) vec;
    res->error = getSuccessInfo();
    return res;
}

ResultInfo *concatVectors(const vector *first, const vector *second) {
    ResultInfo *res = malloc(sizeof(ResultInfo));
    if (first == NULL || second == NULL) {
        res->result = NULL;
        res->error = getErrorInfo(ERROR_NULL_POINTER, "Error: Null pointer to vector.");
        return res;
    }
    if (first->argType != second->argType) {
        res->result = NULL;
        res->error = getErrorInfo(ERROR_TYPE_MISMATCH, "Error: Cannot concat vectors with different types.");
        return res;
    }
    const TypeInfo *type = first->argType;
    vector *vec = (vector *) nElementsVector(first->size + second->size, type)->result;
    size_t width = type->getSize();
    char *ptrA = (char *) first->start;
    for (size_t i = 0; i < first->size; ++i) {
        const void *cur = (const void *) (ptrA + width * i);
        assignElement(vec, i, cur);
    }
    char *ptrB = (char *) second->start;
    for (size_t i = 0; i < second->size; ++i) {
        const void *cur = (const void *) (ptrB + width * i);
        assignElement(vec, i + first->size, cur);
    }
    res->result = (void *) vec;
    res->error = getSuccessInfo();
    return res;
}

const ErrorInfo *print(const vector *v) {
    if (v == NULL) {
        return getErrorInfo(ERROR_NULL_POINTER, "Error: Null pointer to vector.");
    }
    if (v->size == 0) {
        printf("Empty vector\n");
        return getSuccessInfo();
    }
    size_t width = v->argType->getSize();
    char *ptr = (char *) v->start;
    for (size_t i = 0; i < v->size; ++i) {
        v->argType->print((const void *) (ptr + width * i));
        if (i + 1 < v->size) {
            printf(", ");
        }
    }
    printf("\n");
    return getSuccessInfo();
}
