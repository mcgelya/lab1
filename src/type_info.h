#pragma once

#include <stdlib.h>

typedef void (*UnaryOperator)(void *result, const void *arg);
typedef void *(*Allocator)();
typedef size_t (*SizeGetter)();
typedef void (*Print)(const void *arg);

typedef struct TypeInfo {
    UnaryOperator assign;
    Allocator alloc;
    SizeGetter getSize;
    Print print;
} TypeInfo;
