#pragma once

#include <stdbool.h>

bool isEven(const void *arg);

bool isZero(const void *arg);

bool isRe(const void *arg);

bool isIm(const void *arg);

void inc(void *dest, const void *source);

void negate(void *dest, const void *source);

void conjugate(void *dest, const void *source); // сопряженное для комплов
