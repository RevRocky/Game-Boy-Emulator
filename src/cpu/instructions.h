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
void load_register(Register8 *destination, Register8 *source);
void load_register_indirect_destination(Register16 *address_register, Register8 *source);
void load_register_indirect_source(Register8 *destination, Register16 *address_register);
void load_accumulator_from_address(Register8 *accumultator, unsigned short memory_address);
void write_accumulator_to_address(unsigned short memory_address, Register8 *accumulator);
void load_accumulator_decrement_address_register(Register8 *accumulator, Register16 *address_register);
void load_accumulator_increment_address_register(Register8 *accumulator, Register16 *address_register);
void write_accumulator_decrement_address_register(Register16 *address_register, Register8 *accumulator);
void write_accumulator_increment_address_register(Register16 *address_register, Register8 *accumulator);
void load_from_io_port_c(Register8 *accumulator, Register8 *offset_register);
void write_to_io_port_c(Register8 *offset_register, Register8 *accumulator);
void load_from_io_port_n(Register8 *accumulator, unsigned char offset);
void write_to_io_port_n(unsigned char offset, Register8 *accumulator);

// 16 BIT LOADS //

void load_immediate_short(Register16 *destination, unsigned short value);
void load_stack_pointer(Register16 *stack_pointer, Register16 *source_register);
void load_stack_pointer_offset(Register16 *stack_pointer, short offset, Register8 *flags);
void write_stack_pointer_to_address(Register16 *stack_pointer, unsigned short address);
void push(Register16 *stack_pointer, Register16 *source_register);
void pop(Register16 *stack_pointer, Register16 *destination_register);

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

// Set
void set_bit_register(Register8 *target_register, unsigned char bit);
void set_bit_indirect(Register16 *address_register, unsigned char bit);

// Reset
void reset_bit_register(Register8 *target_register, unsigned char bit);
void reset_bit_indirect(Register16 *address_register, unsigned char bit);

// JUMPS //

// Unconditional Jumps
void jump_unconditional(Register16 *programme_counter, unsigned short address_big_endian);
void jump_indirect(Register16 *programme_counter, Register16 *address_register);
void jump_relative_pos(Register16 *programme_counter, unsigned char offset);

// Conditional Jumps
void jump_zero_reset(Register16 *programme_counter, unsigned short address_big_endian, Register8 *flags);
void jump_zero_set(Register16 *programme_counter, unsigned short address_big_endian, Register8 *flags);
void jump_carry_reset(Register16 *programme_counter, unsigned short address_big_endian, Register8 *flags);
void jump_carry_set(Register16 *programme_counter, unsigned short address_big_endian, Register8 *flags);

void jump_relative_zero_reset(Register16 *programme_counter, unsigned char offset, Register8 *flags);
void jump_relative_zero_set(Register16 *programme_counter, unsigned char offset, Register8 *flags);
void jump_relative_carry_reset(Register16 *programme_counter, unsigned char offset, Register8 *flags);
void jump_relative_carry_set(Register16 *programme_counter, unsigned char offset, Register8 *flags);

// CALLS //
void restart(Register16 *stack_pointer, Register16 *programme_counter, unsigned char offset);

// Unconditional
void call(Register16 *stack_pointer, Register16 *programme_counter, unsigned short call_address_big_endian);

// Conditional
void call_zero_reset(Register16 *stack_pointer, Register16 *programme_counter, unsigned short call_address_big_endian, Register8 *flags);
void call_zero_set(Register16 *stack_pointer, Register16 *programme_counter, unsigned short call_address_big_endian, Register8 *flags);
void call_carry_reset(Register16 *stack_pointer, Register16 *programme_counter, unsigned short call_address_big_endian, Register8 *flags);
void call_carry_set(Register16 *stack_pointer, Register16 *programme_counter, unsigned short call_address_big_endian, Register8 *flags);

// RESTARTS //
void restart(Register16 *stack_pointer, Register16 *programme_counter, unsigned char offset);

// RETURNS // 

// Unconditional
void return_unconditional(Register16 *stack_pointer, Register16 *programme_counter);

// Conditional
void return_zero_reset(Register16 *stack_pointer, Register16 *programme_counter, Register8 *flags);
void return_zero_set(Register16 *stack_pointer, Register16 *programme_counter, Register8 *flags);
void return_carry_reset(Register16 *stack_pointer, Register16 *programme_counter, Register8 *flags);
void return_carry_set(Register16 *stack_pointer, Register16 *programme_counter, Register8 *flags);

// MISC //

void swap_nibble_register(Register8 *target_register, Register8 *flags);
void swap_nibble_indirect(Register16 *address_register, Register8 *flags);

void decimal_adjust_accumulator(Register8 *accumulator, Register8 *flags);

void complement_accumulator(Register8 *accumulator, Register8 *flags);
void complement_carry_flag(Register8 *flags);
void set_carry_flag(Register8 *flags);


#endif

