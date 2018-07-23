#include "../cpu/register.h"

#ifndef UTIL_H
#define UTIL_H

    unsigned short to_little_endian(unsigned short big_endian_number);
    int zero_flag_set(Register8 *flags);
    int negative_flag_set(Register8 *flags);
    int half_carry_flag_set(Register8 *flags);
    int carry_flag_set(Register8 *flags);

#endif  // UTIL_H