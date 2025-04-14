#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef void (*UnaryOperator)(void *result, const void *arg);
typedef bool (*EqualityOperator)(const void *a, const void *b);
typedef void *(*Allocator)(size_t n);
typedef size_t (*SizeGetter)();
typedef void (*Print)(const void *arg);

typedef struct TypeInfo {
    UnaryOperator assign;
    EqualityOperator eqOperator;
    Allocator alloc;
    SizeGetter getSize;
    Print print;
} TypeInfo;
