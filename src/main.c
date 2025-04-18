#include "complex.h"
#include "complex_on_integer.h"
#include "example.h"
#include "integer.h"
#include "vector.h"
#include <stdio.h>
#include <string.h>

int main() {
    while (true) {
        printf("Choose mode: i/c (i for integer, c for complex): ");
        fflush(stdin);
        char c;
        scanf("%c", &c);
        if (c == 'i') {
            vector *v = (vector *) emptyVector(getIntegerTypeInfo())->result;
            printf("Let's play. Enter '+ x' to push_back x, '-' to pop_back, or '!' to quit:\n");
            while (true) {
                char c;
                scanf("%c", &c);
                if (c == '+') {
                    Integer x;
                    scanf("%d", &x.value);
                    pushBack(v, &x);
                } else if (c == '-') {
                    ErrorInfo *err = popBack(v);
                    if (err != getSuccessResult()) {
                        printf("%s\n", err->msg);
                        return 0;
                    }
                } else if (c == '!') {
                    break;
                }
            }
            printf("\nGreat! Here's your vector:\n");
            print(v);

            vector *filtered = (vector *) where(v, isEven)->result;
            printf("\nFiltered to leave only even numbers:\n");
            print(filtered);

            vector *mp = (vector *) map(v, inc)->result;
            printf("\nMapped with 'inc', all values increased by one:\n");
            print(mp);

            vector *conc = (vector *) concatVectors(filtered, mp)->result;
            printf("\nConcatenated these vectors:\n");
            print(conc);
        } else if (c == 'c') {
            vector *v = (vector *) emptyVector(getComplexOnIntegerTypeInfo())->result;
            printf("Let's play. Enter '+ re im' to push_back re + i * im, '-' to pop_back, or '!' to quit:\n");
            while (true) {
                char c;
                scanf("%c", &c);
                if (c == '+') {
                    Integer re, im;
                    scanf("%d %d", &re.value, &im.value);
                    Complex *z = getComplexOnIntegerTypeInfo()->alloc(1);
                    getIntegerTypeInfo()->assign(z->re, &re);
                    getIntegerTypeInfo()->assign(z->im, &im);
                    pushBack(v, z);
                } else if (c == '-') {
                    ErrorInfo *err = popBack(v);
                    if (err != getSuccessResult()) {
                        printf("%s\n", err->msg);
                        return 0;
                    }
                } else if (c == '!') {
                    break;
                }
            }
            printf("\nGreat! Here's your vector:\n");
            print(v);

            vector *filtered = (vector *) where(v, isRe)->result;
            printf("\nFiltered to leave only real numbers:\n");
            print(filtered);

            printf("\nMapped with 'conjugate':\n");
            vector *mp = (vector *) map(v, conjugate)->result;
            print(mp);

            vector *conc = (vector *) concatVectors(filtered, mp)->result;
            printf("\nConcatenated these vectors:\n");
            print(conc);
        } else {
            printf("Mode should be i or c\n");
        }
        printf("Try again? y/n: ");
        fflush(stdin);
        char flag;
        scanf("%c", &flag);
        if (flag == 'N' || flag == 'n') {
            break;
        }
    }
}
