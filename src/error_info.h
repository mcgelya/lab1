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
    const char *msg;
} ErrorInfo;

const ErrorInfo *getSuccessInfo();
const ErrorInfo *getErrorInfo(ErrorCode code, const char *msg);

typedef struct ResultInfo {
    void *result;
    const ErrorInfo *error;
} ResultInfo;
