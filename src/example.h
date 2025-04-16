#pragma once

#include <stdbool.h>

bool is_even(const void *arg);

bool is_zero(const void *arg);

bool is_re(const void *arg);

bool is_im(const void *arg);

void inc(void *dest, const void *source);

void negate(void *dest, const void *source);

void conjugate(void *dest, const void *source);
