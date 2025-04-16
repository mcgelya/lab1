#include "error_info.h"

static ErrorInfo *successResult = NULL;

ErrorInfo *getSuccessResult() {
    if (successResult == NULL) {
        successResult = malloc(sizeof(ErrorInfo));
        successResult->code = ERROR_OK;
        successResult->msg = "OK.";
    }
    return successResult;
}
