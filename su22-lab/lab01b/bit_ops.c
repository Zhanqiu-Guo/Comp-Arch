#include <stdint.h>
#include <stdio.h>
#include "bit_ops.h"

/*
  Returns the n-th bit of x.
  Assumes 0 <= n <= 31.
*/
uint32_t get_bit(uint32_t x, uint32_t n) {
    /* YOUR CODE HERE */
    uint32_t bit = (x >> n) & 1;
    return bit; /* UPDATE RETURN VALUE */
}

/*
  Set the n-th bit of *x to v.
  Assumes 0 <= n <= 31, and v is 0 or 1.
*/
void set_bit(uint32_t *x, uint32_t n, uint32_t v) {
    /* YOUR CODE HERE */
    *x = (*x & (~(1 << n))) | (v << n);
}

/*
  Flips the n-th bit of *x.
  Assumes 0 <= n <= 31.
*/
void flip_bit(uint32_t *x, uint32_t n) {
    /* YOUR CODE HERE */
    uint32_t bit = get_bit(*x, n)^1;
    set_bit(x, n, bit);
}

