#include "vector.h"
#include <assert.h>
#include <stdio.h>

ResultInfo *emptyVector(const TypeInfo *argType) {
    ResultInfo *res = malloc(sizeof(ResultInfo));
    if (argType == NULL) {
        ErrorInfo *err = malloc(sizeof(ErrorInfo));
        err->code = ERROR_NULL_POINTER;
        err->msg = "Error: Null pointer to argType.";
        res->result = NULL;
        res->error = err;
        return res;
    }
    vector *vec = malloc(sizeof(vector));
    vec->start = argType->alloc(1);
    vec->size = 0;
    vec->capacity = 1;
    vec->argType = argType;
    res->result = (void *) vec;
    res->error = getSuccessResult();
    return res;
}

ResultInfo *nElementsVector(size_t n, const TypeInfo *argType) {
    ResultInfo *res = malloc(sizeof(ResultInfo));
    if (argType == NULL) {
        ErrorInfo *err = malloc(sizeof(ErrorInfo));
        err->code = ERROR_NULL_POINTER;
        err->msg = "Error: Null pointer to argType.";
        res->result = NULL;
        res->error = err;
        return res;
    }
    vector *vec = malloc(sizeof(vector));
    vec->start = argType->alloc(n);
    vec->size = n;
    vec->capacity = n;
    vec->argType = argType;
    res->result = (void *) vec;
    res->error = getSuccessResult();
    return res;
}

ErrorInfo *push_back(vector *v, const void *source) {
    if (v == NULL) {
        ErrorInfo *err = malloc(sizeof(ErrorInfo));
        err->code = ERROR_NULL_POINTER;
        err->msg = "Error: Null pointer to vector.";
        return err;
    }
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
    return getSuccessResult();
}

ErrorInfo *pop_back(vector *v) {
    if (v == NULL) {
        ErrorInfo *err = malloc(sizeof(ErrorInfo));
        err->code = ERROR_NULL_POINTER;
        err->msg = "Error: Null pointer to vector.";
        return err;
    }
    if (v->size == 0) {
        ErrorInfo *err = malloc(sizeof(ErrorInfo));
        err->code = ERROR_OUT_OF_BOUNDS;
        err->msg = "Error: Cannot pop element from an empty vector.";
        return err;
    }
    --v->size;
    return getSuccessResult();
}

ResultInfo *getElement(vector *v, size_t i) {
    ResultInfo *res = malloc(sizeof(ResultInfo));
    if (v == NULL) {
        ErrorInfo *err = malloc(sizeof(ErrorInfo));
        err->code = ERROR_NULL_POINTER;
        err->msg = "Error: Null pointer to vector.";
        res->result = NULL;
        res->error = err;
        return res;
    }
    if (i >= v->size) {
        ErrorInfo *err = malloc(sizeof(ErrorInfo));
        err->code = ERROR_OUT_OF_BOUNDS;
        err->msg = "Error: index is larger than size.";
        res->result = NULL;
        res->error = err;
        return res;
    }
    size_t width = v->argType->getSize();
    char *ptr = (char *) v->start;
    res->result = (void *) (ptr + width * i);
    res->error = getSuccessResult();
    return res;
}

ErrorInfo *assignElement(vector *v, size_t i, const void *source) {
    if (v == NULL) {
        ErrorInfo *err = malloc(sizeof(ErrorInfo));
        err->code = ERROR_NULL_POINTER;
        err->msg = "Error: Null pointer to vector.";
        return err;
    }
    if (i >= v->size) {
        ErrorInfo *err = malloc(sizeof(ErrorInfo));
        err->code = ERROR_OUT_OF_BOUNDS;
        err->msg = "Error: index is larger than size.";
        return err;
    }
    size_t width = v->argType->getSize();
    char *ptr = (char *) v->start;
    v->argType->assign((void *) (ptr + width * i), source);
    return getSuccessResult();
}

ResultInfo *map(const vector *v, UnaryOperator func) {
    ResultInfo *res = malloc(sizeof(ResultInfo));
    if (v == NULL) {
        ErrorInfo *err = malloc(sizeof(ErrorInfo));
        err->code = ERROR_NULL_POINTER;
        err->msg = "Error: Null pointer to vector.";
        res->result = NULL;
        res->error = err;
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
    res->error = getSuccessResult();
    return res;
}

ResultInfo *where(const vector *v, Predicate pred) {
    ResultInfo *res = malloc(sizeof(ResultInfo));
    if (v == NULL) {
        ErrorInfo *err = malloc(sizeof(ErrorInfo));
        err->code = ERROR_NULL_POINTER;
        err->msg = "Error: Null pointer to vector.";
        res->result = NULL;
        res->error = err;
        return res;
    }
    vector *vec = (vector *) emptyVector(v->argType)->result;
    size_t width = v->argType->getSize();
    char *ptr = (char *) v->start;
    for (size_t i = 0; i < v->size; ++i) {
        const void *cur = (const void *) (ptr + width * i);
        if (pred(cur)) {
            push_back(vec, cur);
        }
    }
    res->result = (void *) vec;
    res->error = getSuccessResult();
    return res;
}

ResultInfo *concatVectors(const vector *first, const vector *second) {
    ResultInfo *res = malloc(sizeof(ResultInfo));
    if (first == NULL || second == NULL) {
        ErrorInfo *err = malloc(sizeof(ErrorInfo));
        err->code = ERROR_NULL_POINTER;
        err->msg = "Error: Null pointer to vector.";
        res->result = NULL;
        res->error = err;
        return res;
    }
    if (first->argType != second->argType) {
        ErrorInfo *err = malloc(sizeof(ErrorInfo));
        err->code = ERROR_TYPE_MISMATCH;
        err->msg = "Error: Cannot concat vectors with different types.";
        res->result = NULL;
        res->error = err;
        return res;
    }
    vector *vec = (vector *) emptyVector(first->argType)->result;
    size_t width = first->argType->getSize();
    char *ptr_a = (char *) first->start;
    for (size_t i = 0; i < first->size; ++i) {
        const void *cur = (const void *) (ptr_a + width * i);
        push_back(vec, cur);
    }
    char *ptr_b = (char *) second->start;
    for (size_t i = 0; i < second->size; ++i) {
        const void *cur = (const void *) (ptr_b + width * i);
        push_back(vec, cur);
    }
    res->result = (void *) vec;
    res->error = getSuccessResult();
    return res;
}

ErrorInfo *print(const vector *v) {
    if (v == NULL) {
        ErrorInfo *err = malloc(sizeof(ErrorInfo));
        err->code = ERROR_NULL_POINTER;
        err->msg = "Error: Null pointer to vector.";
        return err;
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
    return getSuccessResult();
}
