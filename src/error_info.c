#include "error_info.h"

static ErrorInfo *successResult = NULL;

const ErrorInfo *getSuccessInfo() {
    if (successResult == NULL) {
        successResult = malloc(sizeof(ErrorInfo));
        successResult->code = ERROR_OK;
        successResult->msg = "OK.";
    }
    return successResult;
}

const ErrorInfo *getErrorInfo(ErrorCode code, const char *msg) {
    ErrorInfo *res = malloc(sizeof(ErrorInfo));
    res->code = code;
    res->msg = msg;
    return res;
}
