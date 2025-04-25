#include "complex.h"
#include "complex_on_integer.h"
#include "example.h"
#include "integer.h"
#include "vector.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

const size_t filterIntegersNum = 2;
static const Predicate filterIntegers[] = {isEven, isZero};
static const char *filterIntegersNames[] = {"isEven: Keeps only even numbers", "isZero: Keeps only numbers equal to 0"};

const size_t filterComplexNum = 2;
static const Predicate filterComplex[] = {isRe, isIm};
static const char *filterComplexNames[] = {"isRe: Keeps only real numbers", "isIm: Keeps only imaginary numbers"};

const size_t mapIntegersNum = 2;
static const UnaryOperator mapIntegers[] = {inc, negate};
static const char *mapIntegersNames[] = {"inc: Increments number", "negate: Inverts the sign of number"};

const size_t mapComplexNum = 3;
static const UnaryOperator mapComplex[] = {conjugate, getRe, getIm};
static const char *mapComplexNames[] = {"conjugate: Returns complex conjugation", "getRe: Returns real part of complex number", "getIm: Returns imaginary part of complex number"};

bool inputInteger(int l, int r, int *x) {
    char buffer[15]; // с запасом
    scanf("%14s", buffer);
    long long val = 0;
    int length = strlen(buffer);
    bool negate = 0;
    for (int i = 0; i < length; ++i) {
        if (i == 0 && buffer[i] == '-') {
            negate = 1;
            continue;
        }
        if (!isdigit(buffer[i])) {
            return false;
        }
        val = val * 10 + buffer[i] - '0';
    }
    if (negate) {
        val *= -1;
    }
    if (val < l || val > r) {
        return false;
    }
    *x = (int) val;
    return true;
}

#define MAX_VECTORS 50
#define MAX_INTEGER 1000000000
vector *vecs[MAX_VECTORS];

int availableVectors = 0;

int playOneCycle(char mode) { // 0 is quit, 1 is ask for continue, 2 is continue
    int mapFunctionsNum = -1;
    int filterFunctionsNum = -1;
    const TypeInfo *argType = NULL;
    const UnaryOperator *mapFuncs = NULL;
    const char **mapNames = NULL;
    const Predicate *filterFuncs = NULL;
    const char **filterNames = NULL;
    if (mode == 'i') {
        mapFunctionsNum = mapIntegersNum;
        filterFunctionsNum = filterIntegersNum;
        argType = getIntegerTypeInfo();
        mapFuncs = mapIntegers;
        mapNames = mapIntegersNames;
        filterFuncs = filterIntegers;
        filterNames = filterIntegersNames;
    } else {
        mapFunctionsNum = mapComplexNum;
        filterFunctionsNum = filterComplexNum;
        argType = getComplexOnIntegerTypeInfo();
        mapFuncs = mapComplex;
        mapNames = mapComplexNames;
        filterFuncs = filterComplex;
        filterNames = filterComplexNames;
    }
    if (availableVectors == MAX_VECTORS) {
        printf("Already available max number of vectors. Try again with another mode!\n");
        return 0;
    }
    printf("\nAvailable options:\n"
           "1. Input new vector\n"
           "2. Map one of available vectors with one of available functions\n"
           "3. Filter one of available vectors with one of available functions\n"
           "4. Concatenate two of available vectors\n"
           "5. See available vectors\n"
           "6. Quit\n");
    int option = 0;
    fflush(stdin);
    if (!inputInteger(1, 6, &option)) {
        printf("Incorrect option!\n");
        return 1;
    }
    if (option == 1) {
        vector *vec = NULL;
        if (mode == 'i') {
            printf("\nInput your numbers. Enter '+ x' to push_back x, '-' to pop_back, or '!' to quit:\n");
            vec = (vector *) emptyVector(getIntegerTypeInfo())->result;
        } else {
            printf("Input your numbers. Enter '+ re im' to push_back x, '-' to pop_back, or '!' to quit:\n");
            vec = (vector *) emptyVector(getComplexOnIntegerTypeInfo())->result;
        }
        while (true) {
            char c;
            scanf(" %c", &c);
            if (c == '+') {
                if (mode == 'i') {
                    Integer x;
                    if (!inputInteger(-MAX_INTEGER, MAX_INTEGER, &x.value)) {
                        printf("Input too large!\n");
                        return 1;
                    }
                    pushBack(vec, &x);
                } else {
                    Integer re, im;
                    if (!inputInteger(-MAX_INTEGER, MAX_INTEGER, &re.value) || !inputInteger(-MAX_INTEGER, MAX_INTEGER, &im.value)) {
                        printf("Input too large!\n");
                        return 1;
                    }
                    Complex *z = getComplexOnIntegerTypeInfo()->alloc(1);
                    getIntegerTypeInfo()->assign(z->re, &re);
                    getIntegerTypeInfo()->assign(z->im, &im);
                    pushBack(vec, z);
                }
            } else if (c == '-') {
                const ErrorInfo *err = popBack(vec);
                if (err->code != ERROR_OK) {
                    printf("%s", err->msg);
                    return 1;
                }
            } else if (c == '!') {
                break;
            } else {
                printf("Incorrect input!\n");
                return 1;
            }
        }
        vecs[availableVectors] = vec;
        ++availableVectors;
        printf("Great! New vector available\n");
        printf("%d. ", availableVectors);
        print(vec);
    } else if (option == 2) {
        printf("\nHere's available functions:\n");
        for (int i = 0; i < mapFunctionsNum; ++i) {
            printf("%d. %s\n", i + 1, mapNames[i]);
        }
        printf("Here's available vectors:\n");
        for (int i = 0; i < availableVectors; ++i) {
            printf("%d. ", i + 1);
            print(vecs[i]);
        }
        printf("Input index of chosen operation and vector: ");
        fflush(stdin);
        int oper = -1;
        if (!inputInteger(1, mapFunctionsNum, &oper)) {
            printf("Incorrect operation!\n");
            return 1;
        }
        int vec = -1;
        if (!inputInteger(1, availableVectors, &vec)) {
            printf("Incorrect vector!\n");
            return 1;
        }
        --oper;
        --vec;
        vector *res = (vector *) map(vecs[vec], mapFuncs[oper])->result;
        printf("Great! Here's your result. Also added to available vectors:\n");
        vecs[availableVectors] = res;
        ++availableVectors;
        printf("%d. ", availableVectors);
        print(res);
    } else if (option == 3) {
        printf("\nHere's available functions:\n");
        for (int i = 0; i < filterFunctionsNum; ++i) {
            printf("%d. %s\n", i + 1, filterNames[i]);
        }
        printf("Here's available vectors:\n");
        for (int i = 0; i < availableVectors; ++i) {
            printf("%d. ", i + 1);
            print(vecs[i]);
        }
        printf("Input index of chosen operation and vector: ");
        fflush(stdin);
        int oper = -1;
        if (!inputInteger(1, filterFunctionsNum, &oper)) {
            printf("Incorrect operation!\n");
            return 1;
        }
        int vec = -1;
        if (!inputInteger(1, availableVectors, &vec)) {
            printf("Incorrect vector!\n");
            return 1;
        }
        --oper;
        --vec;
        vector *res = (vector *) where(vecs[vec], filterFuncs[oper])->result;
        printf("Great! Here's your result. Also added to available vectors:\n");
        vecs[availableVectors] = res;
        ++availableVectors;
        printf("%d. ", availableVectors);
        print(res);
    } else if (option == 4) {
        printf("\nHere's available vectors:\n");
        for (int i = 0; i < availableVectors; ++i) {
            printf("%d. ", i + 1);
            print(vecs[i]);
        }
        printf("Input two indexes of chosen vectors to concatenate: ");
        fflush(stdin);
        int first = -1;
        if (!inputInteger(1, availableVectors, &first)) {
            printf("Incorrect vector!\n");
            return 1;
        }
        int second = -1;
        if (!inputInteger(1, availableVectors, &second)) {
            printf("Incorrect vector!\n");
            return 1;
        }
        --first;
        --second;
        vector *res = (vector *) concatVectors(vecs[first], vecs[second])->result;
        printf("Great! Here's your result. Also added to available vectors:\n");
        vecs[availableVectors] = res;
        ++availableVectors;
        printf("%d. ", availableVectors);
        print(res);
    } else if (option == 5) {
        printf("\nAvailable %d vectors\n", availableVectors);
        for (int i = 0; i < availableVectors; ++i) {
            printf("%d. ", i + 1);
            print(vecs[i]);
        }
    } else {
        return 0;
    }
    return 2;
}

int main() {
    while (true) {
        printf("\nChoose mode: i/c (i for integer, c for complex): ");
        fflush(stdin);
        char mode;
        scanf(" %c", &mode);
        if (mode == 'i' || mode == 'c') {
            while (true) {
                int res = playOneCycle(mode);
                if (res == 0) {
                    break;
                } else if (res == 1) {
                    printf("\nTry again? y/n: ");
                    fflush(stdin);
                    char flag;
                    scanf(" %c", &flag);
                    if (flag == 'N' || flag == 'n') {
                        break;
                    }
                }
            }
        } else {
            printf("Mode should be 'i' or 'c'\n");
        }
        availableVectors = 0;
        printf("\nWanna try again with another mode? y/n: ");
        fflush(stdin);
        char flag;
        scanf(" %c", &flag);
        if (flag == 'N' || flag == 'n') {
            break;
        }
    }
}
