#include "integer.h"
#include <stdio.h>
#include <string.h>

void assignInteger(void *destination, const void *source) {
    Integer *destination_int = (Integer *) destination;
    const Integer *source_int = (const Integer *) source;
    destination_int->value = source_int->value;
}

size_t getSizeInteger() {
    return sizeof(Integer);
}

void *allocInteger(size_t n) {
    return malloc(getSizeInteger() * n);
}

bool eqOperatorInteger(const void *a, const void *b) {
    const Integer *a_int = (const Integer *) a;
    const Integer *b_int = (const Integer *) b;
    return a_int->value == b_int->value;
}

void printInteger(const void *arg) {
    const Integer *arg_int = (const Integer *) arg;
    printf("%d", arg_int->value);
}

static struct TypeInfo *integerType = NULL;

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
