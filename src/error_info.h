#pragma once

#include <stdlib.h>

typedef enum {
    ERROR_OK,
    ERROR_NULL_POINTER,
    ERROR_OUT_OF_BOUNDS,
    ERROR_TYPE_MISMATCH,
} ErrorCode;

typedef struct ErrorInfo {
    ErrorCode code;
    char *msg;
} ErrorInfo;

ErrorInfo *getSuccessResult();

typedef struct ResultInfo {
    void *result;
    ErrorInfo *error;
} ResultInfo;
