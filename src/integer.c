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

void *allocInteger() {
    return malloc(getSizeInteger());
}

void printInteger(const void *arg) {
    const Integer *arg_int = (const Integer *) arg;
    printf("%d", arg_int->value);
}

static struct TypeInfo *intType = NULL;

const TypeInfo *getIntegerTypeInfo() {
    if (intType == NULL) {
        intType = malloc(sizeof(TypeInfo));
        intType->assign = assignInteger;
        intType->alloc = allocInteger;
        intType->getSize = getSizeInteger;
        intType->print = printInteger;
    }
    return intType;
}
