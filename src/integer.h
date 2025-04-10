#pragma once

#include "type_info.h"

typedef struct Integer {
    int value;
} Integer;

const TypeInfo *getIntegerTypeInfo();
