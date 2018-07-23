/**
 * This module contains a host of functions that serve small 
 * utilitarian purposes through out the programme.
 * 
 * @author: Rocky Petkov
 */

#include "util.h"



/**
 * /brief Converst a big endian short to little endian.
 * 
 * Converts a 16 bit little endian number to a big endian format.
 * 
 * @param big_endian_number: A number in big endian format
 * 
 * @returns: An equivalent number in little endian. 
 */
unsigned short to_little_endian(unsigned short big_endian_number) {
    return (big_endian_number << 8) | (big_endian_number >> 8);
}

/**
 * /brief Checks if the zero flag is set.
 * 
 * @param flags: Pointer to the flags register
 * 
 * @returns: Non Zero if zero flag is set. Zero otherwise.
 */
int zero_flag_set(Register8 *flags) {
    return *flags & 0x80;
}

/**
 * /brief Checks if the negative flag is set.
 * 
 * @param flags: Pointer to the flags register
 * 
 * @returns: Non Zero if negagive flag is set. Zero otherwise.
 */
int negative_flag_set(Register8 *flags) {
    return *flags & 0x40;
}

/**
 * /brief Checks if the half-carry flag is set.
 * 
 * @param flags: Pointer to the flags register
 * 
 * @returns: Non Zero if half-carry flag is set. Zero otherwise.
 */
int half_carry_flag_set(Register8 *flags) {
    return *flags & 0x20;
}

/**
 * /brief Checks if the carry flag is set.
 * 
 * @param flags: Pointer to the flags register
 * 
 * @returns: Non Zero if carry flag is set. Zero otherwise.
 */
int carry_flag_set(Register8 *flags) {
    return *flags & 0x10;
}