/**
 * A header file containing definitions for all of the myriad instructions
 * in the emulated Z80-GB instruction set
 *
 * Authors: Rocky Petkov & Alexander Weatherhead
 */

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "register.h"

// See instructions.c for more thorough explination of these functions 

// 8 BIT LOADS // 
void load_immediate_byte(Register8 *destination, unsigned char value);

// 8-BIT ALU //

// Adds
void add_register(Register8 *accumulator, Register8 *other_register, Register8 *flags);
void add_immediate(Register8 *accumulator, unsigned char value, Register8 *flags);
void add_indirect(Register8 *accumulator, Register16 *address_register, Register8 *flags);
void add_register_with_carry(Register8 *accumulator, Register8 *other_register, Register8 *flags);
void add_immediate_with_carry(Register8 *accumulator, unsigned char value, Register8 *flags);
void add_indirect_with_carry(Register8 *accumulator, Register16 *address_register, Register8 *flags);

// Subtracts
void subtract_register(Register8 *accumulator, Register8 *other_register, Register8 *flags);
void subtract_immediate(Register8 *accumulator, unsigned char value, Register8 *flags);
void subtract_indirect(Register8 *accumulator, Register16 *address_register, Register8 *flags);
void subtract_register_with_carry(Register8 *accumulator, Register8 *other_register, Register8 *flags);
void subtract_immediate_with_carry(Register8 *accumulator, unsigned char value, Register8 *flags);
void subtract_indirect_with_carry(Register8 *accumulator, Register16 *address_register, Register8 *flags);

// Bitwise Operators
// And
void bitwise_and_register(Register8 *accumulator, Register8 *other_register, Register8 *flags);
void bitwise_and_immediate(Register8 *accumulator, unsigned char value, Register8 *flags);
void bitwise_and_indirect(Register8 *accumulator, Register16 *address_register, Register8 *flags);

// Or
void bitwise_or_register(Register8 *accumulator, Register8 *other_register, Register8 *flags);
void bitwise_or_immediate(Register8 *accumulator, unsigned char value, Register8 *flags);
void bitwise_or_indirect(Register8 *accumulator, Register16 *address_register, Register8 *flags);

// Xor
void bitwise_xor_register(Register8 *accumulator, Register8 *other_register, Register8 *flags);
void bitwise_xor_immediate(Register8 *accumulator, unsigned char value, Register8 *flags);
void bitwise_xor_indirect(Register8 *accumulator, Register16 *address_register, Register8 *flags);

// Comparisons
void compare_register(Register8 *accumulator, Register8 *other_register, Register8 *flags);
void compare_immediate(Register8 *accumulator, unsigned char value, Register8 *flags);
void compare_indirect(Register8 *accumulator, Register16 *address_register, Register8 *flags);


// Increments

void increment_register(Register8 *target_register, Register8 *flags);
void increment_register_indirect(Register16 *target_register, Register8 *flags);

// Decrements
	
void decrement_register(Register8 *target_register, Register8 *flags);
void decrement_register_indirect(Register16 *target_register, Register8 *flags);


//16-BIT ALU //

// Adds

void indirect_register_add(Register16 *indirect_address_register, Register16 *other_register, Register8 *flags);
void stack_pointer_add(Register16 *stack_pointer, unsigned char value, Register8 *flags);

// Increments & Decrements

void increment_register_16(Register16 *target_register);
void decrement_register_16(Register16 *target_register);

// ROTATES, SHIFTS & ETC //


// Rotates
// Left
void rotate_register_left_carry_archive(Register8 *target_register, Register8 *flags);
void rotate_indirect_left_carry_archive(Register16 *address_register, Register8 *flags);
void rotate_register_left_through_carry(Register8 *target_register, Register8 *flags);
void rotate_indirect_left_through_carry(Register16 *address_register, Register8 *flags);

// Right
void rotate_register_right_carry_archive(Register8 *target_register, Register8 *flags);
void rotate_indirect_right_carry_archive(Register16 *address_register, Register8 *flags);
void rotate_register_right_through_carry(Register8 *target_register, Register8 *flags);
void rotate_indirect_right_through_carry(Register16 *address_register, Register8 *flags);

// Shifts
// Left
void shift_register_left(Register8 *target_register, Register8 *flags);
void shift_indirect_left(Register16 *address_register, Register8 *flags);

// Right
void arithmetic_shift_register_right(Register8 *target_register, Register8 *flags);
void arithmetic_shift_indirect_right(Register16 *address_register, Register8 *flags);
void logical_shift_register_right(Register8 *target_register, Register8 *flags);
void logical_shift_indirect_right(Register16 *address_register, Register8 *flags);

// BIT OPERANDS //

// Test
void test_bit_register(Register8 *target_register, unsigned char *bit, Register8 *flags);
void test_bit_indirect(Register16 *address_register, unsigned char bit, Register8 *flags);

// Set;
void set_bit_register(Register8 *target_register, unsigned char bit);

// Reset

// MISC //

void swap_nibble_register(Register8 *target_register, Register8 *flags);
void swap_nibble_indirect(Register16 *address_register, Register8 *flags);

void decimal_adjust_accumulator(Register8 *accumulator, Register8 *flags);

void complement_accumulator(Register8 *accumulator, Register8 *flags);
void complement_carry_flag(Register8 *flags);
void set_carry_flag(Register8 *flags);


#endif
