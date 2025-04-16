#include "complex_on_integer.h"
#include "complex.h"
#include "integer.h"

static TypeInfo *complexOnIntegerType = NULL;

void assignComplexOnInteger(void *destination, const void *source) {
    assignComplex(destination, source, getIntegerTypeInfo());
}

void *allocComplexOnInteger(size_t n) {
    return allocComplex(n, getIntegerTypeInfo());
}

void printComplexOnInteger(const void *arg) {
    printComplex(arg, getIntegerTypeInfo());
}

const TypeInfo *getComplexOnIntegerTypeInfo() {
    if (complexOnIntegerType == NULL) {
        complexOnIntegerType = malloc(sizeof(TypeInfo));
        complexOnIntegerType->assign = assignComplexOnInteger;
        complexOnIntegerType->alloc = allocComplexOnInteger;
        complexOnIntegerType->getSize = getSizeComplex;
        complexOnIntegerType->print = printComplexOnInteger;
    }
    return complexOnIntegerType;
}
