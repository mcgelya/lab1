#include "integer.h"
#include <stdio.h>

void assignInteger(void *destination, const void *source) {
    Integer *destinationInt = (Integer *) destination;
    const Integer *sourceInt = (const Integer *) source;
    destinationInt->value = sourceInt->value;
}

size_t getSizeInteger() {
    return sizeof(Integer);
}

void *allocInteger(size_t n) {
    return malloc(getSizeInteger() * n);
}

bool eqOperatorInteger(const void *a, const void *b) {
    const Integer *aInt = (const Integer *) a;
    const Integer *bInt = (const Integer *) b;
    return aInt->value == bInt->value;
}

void printInteger(const void *arg) {
    const Integer *argInt = (const Integer *) arg;
    printf("%d", argInt->value);
}

static TypeInfo *integerType = NULL;

const TypeInfo *getIntegerTypeInfo() {
    if (integerType == NULL) {
        integerType = malloc(sizeof(TypeInfo));
        integerType->assign = assignInteger;
        integerType->eqOperator = eqOperatorInteger;
        integerType->alloc = allocInteger;
        integerType->getSize = getSizeInteger;
        integerType->print = printInteger;
    }
    return integerType;
}
