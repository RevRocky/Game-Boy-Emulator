/*
 * This here module tests the functionality of the ALU to ensure 
 * correct operation!
 */

#include "memory/memory.h"
#include "cpu/register.h"
#include "cpu/instructions.h"

#include <stdlib.h>
#include <stdio.h>

#define GP_REGISTER_COUNT 6

// Global variables to track amount of passing and failed tests
int successes;
int failures;

/**
 * /brief Associates register and flag values from a test
 *
 * A handy little struct which allows us to neatly pass into
 * and refer to both the result of an operation and the associated
 * status flags.
 */
typedef struct {
	unsigned char result;
	unsigned char flags;
} TestResult;

/**
 * /brief Associates register and flag values from a test of the 
 * 		16 bit ALU
 *
 * A handy little struct like the one above. The only difference is that
 * the result is a 16-bit value rather than an 8 bit value
 */
typedef struct {
	unsigned short result;
	unsigned char flags;
} TestResult16;

void test_adds();
void test_subtracts();
void test_and();
void test_or();
void test_xor();
void test_increments();
void test_decrements();
void test_compares();

void test_add_16();
void test_increment_16();
void test_decrement_16();

void test_rotate_left();
void test_rotate_right();
void test_left_shifts();
void test_right_shifts();
void test_nibble_swap();

void test_test_bit();
void test_complements();

void check_result(TestResult *expected, TestResult *actual);
TestResult create_test_result(Register8 result, Register8 flags);
void check_result_16(TestResult16 *expected, TestResult16 *actual);

unsigned char *memory_space = NULL;		// Since it's defined in global scope, it must be constant

void main() {
	memory_space = calloc(50, sizeof(char));
	
	successes = 0;
	failures = 0;

	printf("Testing 8-bit ALU\n\n");
	test_adds();
	test_subtracts();
	test_and();
	test_or();
	test_xor();
	test_increments();
	test_decrements();
	test_compares();

	printf("Testing 16-bit ALU\n\n");
	test_add_16();
	test_increment_16();
	test_decrement_16();

	printf("Test Rotates & Swaps\n\n");
	test_rotate_left();
	test_rotate_right();
	test_left_shifts();
	test_right_shifts();
	test_nibble_swap();

	printf("Test Bit Instructions\n\n");
	test_test_bit();
	test_complements();

	free(memory_space);

	printf("\n\nTESTING COMPLETE!\n\t%d Successes\n\t%d Failures\n", successes, failures);
}

// Guess what? It tests the add operations. 
void test_adds() {
	int i;
	TestResult expected;	// Holds the expected results
	TestResult actual;		// Holds the actual results 

	CPUState *registers = initialise_registers();	// Initialise our registers.
	
	// Allows us to quicklyrun tests on all of our general purpose registers.
	Register8 gp_registers[6] = {registers->B, registers->C, registers->D, 
		registers->E, registers->H, registers->L};

	printf("Conducting Tests of 8 bit addition\n");
	printf("Test: A + A (when A = 0)\n");

	// Add 0 to 0 in accumulator
	add_register(&(registers->A), &(registers->A), &(registers->F));
	expected = (TestResult){.result = 0, .flags = 0x80};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	for (i = 0; i < GP_REGISTER_COUNT; i++) {
		printf("Test: A + GP[i] (GP[i] is the ith gen purpose register\n");
		load_immediate_byte(&(registers->A), 0);
		load_immediate_byte(&(gp_registers[i]), 8);	// Let's load 8... Why not?
		add_register(&(registers->A), &(gp_registers[i]), &(registers->F));

		// Reconstruct our structs
		expected = (TestResult){.result = 8, .flags = 0x00};
		actual = (TestResult){.result = registers->A, .flags = registers->F};
		check_result(&expected, &actual);
	}

	// We now move on to checking whether or not each of the flags is correctly set.
	// Since we've checked the 0 flag before, we can now simply 
	// check the carry and half carry flags!
	
	// We'll also check adding immediate values! Whew laddie!
	printf("Test: Immediate Value + Throwing the Half Carry Flag\n");
	load_immediate_byte(&(registers->A), 87);
	add_immediate(&(registers->A), 9, &(registers->F));
	expected = (TestResult){.result = 96, .flags = 0x20};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	// Testing the carry flag.
	printf("Test: Immediate Operand. Throwing the Carry Flag\n");
	load_immediate_byte(&(registers->A), 128);
	add_immediate(&(registers->A), 129, &(registers->F));
	expected = (TestResult){.result = 1, .flags = 0x10};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	// Now we check all three flags being set.
	printf("Test: Immediate Operand. Throwing Zero, Carry, Half Carry Flag\n");
	load_immediate_byte(&(registers->A), 255);
	add_immediate(&(registers->A), 1, &(registers->F));
	expected = (TestResult){.result = 0, .flags = 0xB0};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	// Now we do tests using indirect addressing. 
	printf("Test: Indirect Addressing. Throwing No Flags.\n");
	load_immediate_byte(&(registers->A), 0);
	memory_space[25] = 50;
	registers->HL = 25;
	add_indirect(&(registers->A), &(registers->HL), &(registers->F));
	expected = (TestResult){.result = 50, .flags = 0x00};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Conducting Tests of 8 bit addition with a carry\n");
	printf("Test: Add with Two Registers. Carry set to 0.\n");
	load_immediate_byte(&(registers->A), 0);	// Reset Accumulator
	load_immediate_byte(&(registers->B), 8);
	registers->F = 0x00;						// I could do a load but it'd feel dirty.
	add_register_with_carry(&(registers->A), &(registers->B), &(registers->F));
	expected = (TestResult){.result = 8, .flags = 0x00};
	actual = (TestResult){.result = registers->A, .flags =  registers->F};
	check_result(&expected, &actual);

	printf("Test: Add with two registers. Carry set to 1\n");
	load_immediate_byte(&(registers->A), 0);	// Reset Accumulator
	registers->F = 0x10;
	add_register_with_carry(&(registers->A), &(registers->B), &(registers->F));
	expected = (TestResult){.result = 9, .flags = 0x00};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Test: Add with immediate value. Carry set to 0\n");
	load_immediate_byte(&(registers->A), 17);	// Reset Accumulator
	registers->F = 0x00;
	add_immediate_with_carry(&(registers->A), 100, &(registers->F));
	expected = (TestResult){.result = 117, .flags = 0x00};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Test: Add immediate value. Carry set to 1\n");
	load_immediate_byte(&(registers->A), 17);	// Reset Accumulator
	registers->F = 0x10;
	add_immediate_with_carry(&(registers->A), 100, &(registers->F));
	expected = (TestResult){.result = 118, .flags = 0x00};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Test: Add indrirect value. Carry set to 0. Zero and Carry Thrown\n");
	load_immediate_byte(&(registers->A), 130);	// Reset Accumulator
	registers->F = 0x00;
	memory_space[30] = 138;
	registers->HL = 30;
	add_indirect_with_carry(&(registers->A), &(registers->HL), &(registers->F));
	expected = (TestResult){.result = 12, .flags = 0x10};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	// Note: We simply ignore driving the carry to 1, it was thrown last time!
	printf("Test: Add indrirect value. Carry set to 1.\n");
	load_immediate_byte(&(registers->A), 130);	// Reset Accumulator
	add_indirect_with_carry(&(registers->A), &(registers->HL), &(registers->F));
	expected = (TestResult){.result = 13, .flags = 0x10};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);
	free(registers);
}

// Guess what? It tests the subtraction operations. 
void test_subtracts() {
	int i;
	TestResult expected;	// Holds the expected results
	TestResult actual;		// Holds the actual results 

	CPUState *registers = initialise_registers();	// Initialise our registers.
	
	// Allows us to quicklyrun tests on all of our general purpose registers.
	Register8 gp_registers[6] = {registers->B, registers->C, registers->D, 
		registers->E, registers->H, registers->L};

	printf("Conducting Tests of 8 bit subtraction\n");
	printf("Test: A - A (when A = 0)\n");

	// Add 0 to 0 in accumulator
	subtract_register(&(registers->A), &(registers->A), &(registers->F));
	expected = (TestResult){.result = 0, .flags = 0xC0};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	// For each of the GP registers we will test 31 - 15
	for (i = 0; i < GP_REGISTER_COUNT; i++) {
		printf("Test: A - GP[i] (GP[i] is the ith gen purpose register\nNote: Each Test is performing 31 - 15\n");
		load_immediate_byte(&(gp_registers[i]), 15);	// Let's load 8... Why not?
		load_immediate_byte(&(registers->A), 31);
		subtract_register(&(registers->A), &(gp_registers[i]), &(registers->F));

		// Reconstruct our structs
		expected = (TestResult){.result = 16, .flags = 0x40};
		actual = (TestResult){.result = registers->A, .flags = registers->F};
		check_result(&expected, &actual);
	}

	// We now move on to checking whether or not each of the flags is correctly set.
	// Since we've checked the 0 flag before, we can now simply 
	// check the carry and half carry flags!
	
	// We'll also check adding immediate values! Whew laddie!
	printf("Test: Immediate Value, Throwing the Half Carry Flag\n");
	load_immediate_byte(&(registers->A), 135);
	subtract_immediate(&(registers->A), 8, &(registers->F));
	expected = (TestResult){.result = 127, .flags = 0x60};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	// Testing the carry flag.
	printf("Test: Immediate Operand. Throwing the Carry Flag\n");
	load_immediate_byte(&(registers->A), 8);
	subtract_immediate(&(registers->A), 136, &(registers->F));
	expected = (TestResult){.result = 128, .flags = 0x50};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	// Now we check all three flags being set.
	printf("Test: Immediate Operand. Throwing Carry, Half Carry Flag\n");
	load_immediate_byte(&(registers->A), 1);
	subtract_immediate(&(registers->A), 2, &(registers->F));
	expected = (TestResult){.result = 255, .flags = 0x70};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	// Now we do tests using indirect addressing. 
	printf("Test: Indirect Addressing. Half Carry Thrown.\n");
	load_immediate_byte(&(registers->A), 50);
	memory_space[25] = 25;
	registers->HL = 25;
	subtract_indirect(&(registers->A), &(registers->HL), &(registers->F));
	expected = (TestResult){.result = 25, .flags = 0x60};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Conducting Tests of 8 bit subtraction with a carry\n");
	printf("Test: Subtract with Two Registers. Carry set to 0.\n");
	load_immediate_byte(&(registers->A), 8);	// Reset Accumulator
	load_immediate_byte(&(registers->B), 0);
	registers->F = 0x00;						// I could do a load but it'd feel dirty.
	subtract_register_with_carry(&(registers->A), &(registers->B), &(registers->F));
	expected = (TestResult){.result = 8, .flags = 0x40};
	actual = (TestResult){.result = registers->A, .flags =  registers->F};
	check_result(&expected, &actual);

	printf("Test: Subtract with two registers. Carry set to 1\n");
	load_immediate_byte(&(registers->A), 8);	// Reset Accumulator
	registers->F = 0x10;
	subtract_register_with_carry(&(registers->A), &(registers->B), &(registers->F));
	expected = (TestResult){.result = 7, .flags = 0x40};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Test: Subtract with immediate value. Carry set to 0\n");
	load_immediate_byte(&(registers->A), 50);	// Reset Accumulator
	registers->F = 0x00;
	subtract_immediate_with_carry(&(registers->A), 25, &(registers->F));
	expected = (TestResult){.result = 25, .flags = 0x60};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Test: Subtract immediate value. Carry set to 1\n");
	load_immediate_byte(&(registers->A), 50);	// Reset Accumulator
	registers->F = 0x10;
	subtract_immediate_with_carry(&(registers->A), 25, &(registers->F));
	expected = (TestResult){.result = 24, .flags = 0x60};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Test: Subtract indrirect value. Carry set to 0. Carry and Half Carry Thrown\n");
	load_immediate_byte(&(registers->A), 1);	// Reset Accumulator
	registers->F = 0x00;
	memory_space[30] = 2;
	registers->HL = 30;
	subtract_indirect_with_carry(&(registers->A), &(registers->HL), &(registers->F));
	expected = (TestResult){.result = 255, .flags = 0x70};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	// Note: We simply ignore driving the carry to 1, it was thrown last time!
	printf("Test: Subtract indrirect value. Carry set to 1.\n");
	load_immediate_byte(&(registers->A), 1);	// Reset Accumulator
	subtract_indirect_with_carry(&(registers->A), &(registers->HL), &(registers->F));
	expected = (TestResult){.result = 254, .flags = 0x70};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	free(registers);
}

// Tests the bitwise and. 
void test_and() {
	int i;
	TestResult expected;	// Holds the expected results
	TestResult actual;		// Holds the actual results 

	CPUState *registers = initialise_registers();	// Initialise our registers.
	
	// Allows us to quicklyrun tests on all of our general purpose registers.
	Register8 gp_registers[6] = {registers->B, registers->C, registers->D, 
		registers->E, registers->H, registers->L};

	printf("Conducting Tests of Bitwise AND\n");
	printf("Test: A & A (when A = 0)\n");

	// Add 0 to 0 in accumulator
	bitwise_and_register(&(registers->A), &(registers->A), &(registers->F));
	expected = (TestResult){.result = 0, .flags = 0xA0};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	// For each of the GP registers we will test 31 - 15
	for (i = 0; i < GP_REGISTER_COUNT; i++) {
		printf("Test: A & GP[i] (GP[i] is the ith gen purpose register\nNote: Each Test is performing 8 & 15\n");
		load_immediate_byte(&(gp_registers[i]), 8);	// Let's load 8... Why not?
		load_immediate_byte(&(registers->A), 15);
		bitwise_and_register(&(registers->A), &(gp_registers[0]), &(registers->F));

		// Reconstruct our structs
		expected = (TestResult){.result = 8, .flags = 0x20};
		actual = (TestResult){.result = registers->A, .flags = registers->F};
		check_result(&expected, &actual);
	}

	
	// We'll let us check with an direct value
	printf("Test: Bitwise And Immediate Value, Throwing the Half Carry Flag\n");
	load_immediate_byte(&(registers->A), 103);
	bitwise_and_immediate(&(registers->A), 76, &(registers->F));
	expected = (TestResult){.result = 68, .flags = 0x20};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Test: Bitwise And indrirect value. Half Carry Thrown\n");
	load_immediate_byte(&(registers->A), 88);	// Reset Accumulator
	memory_space[30] = 30;
	registers->HL = 30;
	bitwise_and_indirect(&(registers->A), &(registers->HL), &(registers->F));
	expected = (TestResult){.result = 24, .flags = 0x20};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);
	free(registers);
}

// Testing the bitwise OR
void test_or(){
	int i;
	TestResult expected;	// Holds the expected results
	TestResult actual;		// Holds the actual results 

	CPUState *registers = initialise_registers();	// Initialise our registers.
	
	// Allows us to quicklyrun tests on all of our general purpose registers.
	Register8 gp_registers[6] = {registers->B, registers->C, registers->D, 
		registers->E, registers->H, registers->L};

	printf("Conducting Tests of Bitwise OR\n");
	printf("Test: A | A (when A = 0)\n");

	// Add 0 to 0 in accumulator
	bitwise_or_register(&(registers->A), &(registers->A), &(registers->F));
	expected = (TestResult){.result = 0, .flags = 0x80};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	// For each of the GP registers we will test 31 - 15
	for (i = 0; i < GP_REGISTER_COUNT; i++) {
		printf("Test: A | GP[i] (GP[i] is the ith gen purpose register\nNote: Each Test is performing 8 & 15\n");
		load_immediate_byte(&(gp_registers[i]), 8);	// Let's load 8... Why not?
		load_immediate_byte(&(registers->A), 15);
		bitwise_or_register(&(registers->A), &(gp_registers[0]), &(registers->F));

		// Reconstruct our structs
		expected = (TestResult){.result = 15, .flags = 0x00};
		actual = (TestResult){.result = registers->A, .flags = registers->F};
		check_result(&expected, &actual);
	}

	
	// We'll let us check with an direct value
	printf("Test: Bitwise OR Immediate Value\n");
	load_immediate_byte(&(registers->A), 8);
	bitwise_or_immediate(&(registers->A), 15, &(registers->F));
	expected = (TestResult){.result = 15, .flags = 0x00};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Test: Bitwise OR indrirect value\n");
	load_immediate_byte(&(registers->A), 88);	// Reset Accumulator
	memory_space[30] = 2;
	registers->HL = 30;
	bitwise_or_indirect(&(registers->A), &(registers->HL), &(registers->F));
	expected = (TestResult){.result = 90, .flags = 0x00};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);
	free(registers);
}

void test_xor() {
	int i;
	TestResult expected;	// Holds the expected results
	TestResult actual;		// Holds the actual results 

	CPUState *registers = initialise_registers();	// Initialise our registers.
	
	// Allows us to quicklyrun tests on all of our general purpose registers.
	Register8 gp_registers[6] = {registers->B, registers->C, registers->D, 
		registers->E, registers->H, registers->L};

	printf("Conducting Tests of Bitwise XOR\n");
	printf("Test: A ^ A (when A = 0)\n");

	// Add 0 to 0 in accumulator
	bitwise_xor_register(&(registers->A), &(registers->A), &(registers->F));
	expected = (TestResult){.result = 0, .flags = 0x80};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	// For each of the GP registers we will test 31 - 15
	for (i = 0; i < GP_REGISTER_COUNT; i++) {
		printf("Test: A ^ GP[i] (GP[i] is the ith gen purpose register\nNote: Each Test is performing 8 & 15\n");
		load_immediate_byte(&(gp_registers[i]), 8);	// Let's load 8... Why not?
		load_immediate_byte(&(registers->A), 15);
		bitwise_xor_register(&(registers->A), &(gp_registers[0]), &(registers->F));

		// Reconstruct our structs
		expected = (TestResult){.result = 7, .flags = 0x00};
		actual = (TestResult){.result = registers->A, .flags = registers->F};
		check_result(&expected, &actual);
	}

	
	// We'll let us check with an direct value
	printf("Test: Bitwise XOR Immediate Value. Throw the 0 flag\n");
	load_immediate_byte(&(registers->A), 8);
	bitwise_xor_immediate(&(registers->A), 8, &(registers->F));
	expected = (TestResult){.result = 0, .flags = 0x80};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Test: Bitwise XOR indrirect value\n");
	load_immediate_byte(&(registers->A), 88);	// Reset Accumulator
	memory_space[30] = 30;
	registers->HL = 30;
	bitwise_xor_indirect(&(registers->A), &(registers->HL), &(registers->F));
	expected = (TestResult){.result = 70, .flags = 0x00};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);
	free(registers);
}

// Testing Incrementation 
void test_increments() {
	int i;
	TestResult expected;	// Holds the expected results
	TestResult actual;		// Holds the actual results 

	CPUState *registers = initialise_registers();	// Initialise our registers.
	
	// Allows us to quicklyrun tests on all of our general purpose registers.
	Register8 gp_registers[6] = {registers->B, registers->C, registers->D, 
		registers->E, registers->H, registers->L};

	printf("Conducting Tests of Incrementation\n");
	printf("Incrementing 0 by 1. No flags raised\n");
	load_immediate_byte(&(registers->A), 0);
	increment_register(&(registers->A), &(registers->F));
	expected = (TestResult){.result = 1, .flags = 0x00};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Incrementing 15 by 1. Throwing half carry flag\n");
	load_immediate_byte(&(registers->B), 15);
	increment_register(&(registers->B), &(registers->F));
	expected = (TestResult){.result = 16, .flags = 0x20};
	actual = (TestResult){.result = registers->B, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Incrementing 255 by 1. Throwing the Zero, Carry and Half Carry Flag\n");
	load_immediate_byte(&(registers->C), 255);
	increment_register(&(registers->C), &(registers->F));
	expected = (TestResult){.result = 0, .flags = 0xB0};
	actual = (TestResult){.result = registers->C, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Testing Indirect Incrementation\n");
	memory_space[21] = 6;
	registers->HL = 21;
	increment_register_indirect(&(registers->HL), &(registers->F));
	expected = (TestResult){.result = 7, .flags = 0x00};
	actual = (TestResult){.result = memory_space[21], .flags = registers->F};
	check_result(&expected, &actual);
	free(registers);
}

// Testing Decrementation
void test_decrements() {
	int i;
	TestResult expected;	// Holds the expected results
	TestResult actual;		// Holds the actual results 

	CPUState *registers = initialise_registers();	// Initialise our registers.
	
	// Allows us to quicklyrun tests on all of our general purpose registers.
	Register8 gp_registers[6] = {registers->B, registers->C, registers->D, 
		registers->E, registers->H, registers->L};

	printf("Conducting Tests of Decrementation\n");
	printf("Decrementing 0 by 1. All flags raised (except zero)\n");
	load_immediate_byte(&(registers->A), 0);
	decrement_register(&(registers->A), &(registers->F));
	expected = (TestResult){.result = 255, .flags = 0x70};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Decrementing 17 by 1. Throwing subtraction flag\n");
	load_immediate_byte(&(registers->B), 17);
	decrement_register(&(registers->B), &(registers->F));
	expected = (TestResult){.result = 16, .flags = 0x40};
	actual = (TestResult){.result = registers->B, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Incrementing 1 by 1. Throwing the Zero and Subtraction Flag\n");
	load_immediate_byte(&(registers->C), 1);
	decrement_register(&(registers->C), &(registers->F));
	expected = (TestResult){.result = 0, .flags = 0xC0};
	actual = (TestResult){.result = registers->C, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Testing Indirect Incrementation\n");
	memory_space[21] = 6;
	registers->HL = 21;
	decrement_register_indirect(&(registers->HL), &(registers->F));
	expected = (TestResult){.result = 5, .flags = 0x40};
	actual = (TestResult){.result = memory_space[21], .flags = registers->F};
	check_result(&expected, &actual);
	free(registers);
}

void test_compares() {
	int i;
	TestResult expected;	// Holds the expected results
	TestResult actual;		// Holds the actual results 

	CPUState *registers = initialise_registers();	// Initialise our registers.
	
	// Allows us to quicklyrun tests on all of our general purpose registers.
	Register8 gp_registers[6] = {registers->B, registers->C, registers->D, 
		registers->E, registers->H, registers->L};

	printf("Conducting Tests of Comparisons\n");
	printf("Test: cmp(A, A) A = 0\n");

	// Add 0 to 0 in accumulator
	compare_register(&(registers->A), &(registers->A), &(registers->F));
	expected = (TestResult){.result = 0, .flags = 0xC0};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	// For each of the GP registers we will test cmp(31, 15)
	for (i = 0; i < GP_REGISTER_COUNT; i++) {
		printf("Test: cmp(A, GP[i]) (GP[i] is the ith gen purpose register\nNote: Each Test is performing 31 - 15\n");
		load_immediate_byte(&(gp_registers[i]), 15);	// Let's load 8... Why not?
		load_immediate_byte(&(registers->A), 31);
		compare_register(&(registers->A), &(gp_registers[i]), &(registers->F));

		// Reconstruct our structs
		expected = (TestResult){.result = 31, .flags = 0x40};
		actual = (TestResult){.result = registers->A, .flags = registers->F};
		check_result(&expected, &actual);
	}

	// We now move on to checking whether or not each of the flags is correctly set.
	// Since we've checked the 0 flag before, we can now simply 
	// check the carry and half carry flags!
	
	// We'll also check adding immediate values! Whew laddie!
	printf("Test: Immediate Value, Throwing the Half Carry Flag\n");
	load_immediate_byte(&(registers->A), 135);
	compare_immediate(&(registers->A), 8, &(registers->F));
	expected = (TestResult){.result = 135, .flags = 0x60};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	// Testing the carry flag.
	printf("Test: Immediate Operand. Throwing the Carry Flag\n");
	load_immediate_byte(&(registers->A), 8);
	compare_immediate(&(registers->A), 136, &(registers->F));
	expected = (TestResult){.result = 8, .flags = 0x50};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	// Now we check all three flags being set.
	printf("Test: Immediate Operand. Throwing Carry, Half Carry Flag\n");
	load_immediate_byte(&(registers->A), 1);
	compare_immediate(&(registers->A), 2, &(registers->F));
	expected = (TestResult){.result = 1, .flags = 0x70};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	// Now we do tests using indirect addressing. 
	printf("Test: Indirect Addressing. Half Carry Thrown.\n");
	load_immediate_byte(&(registers->A), 50);
	memory_space[25] = 25;
	registers->HL = 25;
	compare_indirect(&(registers->A), &(registers->HL), &(registers->F));
	expected = (TestResult){.result = 50, .flags = 0x60};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);
	free(registers);
}

// Tests for the 16-bit ALU

// Tests the 16 bit add
void test_add_16() {
	TestResult16 expected;	// Holds the expected results
	TestResult16 actual;	// Holds the actual results 

	CPUState *registers = initialise_registers();

	printf("Testing 16 bit adds where the \"HL\" register is the destination\n");
	printf("Test: HL += HL. HL is set to 0. 0 flag is not thrown\n");
	registers->HL = 0;
	indirect_register_add(&(registers->HL), &(registers->HL), &(registers->F));
	expected = (TestResult16){.result = 0, .flags = 0x00};
	actual = (TestResult16){.result = registers->HL, .flags = registers->F};
	check_result_16(&expected, &actual);

	printf("Test: HL + BC. Thowing the half carry flag\n");
	registers->HL = 4095;
	registers->BC = 1;
	indirect_register_add(&(registers->HL), &(registers->BC), &(registers->F));
	expected = (TestResult16){.result = 4096, .flags = 0x20};
	actual = (TestResult16){.result = registers->HL, .flags = registers->F};
	check_result_16(&expected, &actual);

	printf("Test: HL += DE. Throwing the carry and half carry flag\n");
	registers->HL = 65301;
	registers->DE = 1031;
	indirect_register_add(&(registers->HL), &(registers->DE), &(registers->F));
	expected = (TestResult16){.result = 796, .flags = 0x30};
	actual = (TestResult16){.result = registers->HL, .flags = registers->F};
	check_result_16(&expected, &actual);

	printf("Test: HL += SP. Throwing the carry flag.\n");
	registers->HL = 65534;
	registers->SP = 4097;
	indirect_register_add(&(registers->HL), &(registers->SP), &(registers->F));
	expected = (TestResult16){.result = 4095, .flags = 0x10};
	actual = (TestResult16){.result = registers->HL, .flags = registers->F};
	check_result_16(&expected, &actual);

	printf("Testing 16 bit adds where the \"SP\" register is the destination\n");
	printf("Test: SP += SP. SP is set to 0. 0 flag is not thrown\n");
	registers->SP = 0;
	stack_pointer_add(&(registers->SP), 0, &(registers->F));
	expected = (TestResult16){.result = 0, .flags = 0x00};
	actual = (TestResult16){.result = registers->SP, .flags = registers->F};
	check_result_16(&expected, &actual);

	printf("Test: Thowing the half carry flag\n");
	registers->SP = 4095;
	stack_pointer_add(&(registers->SP), 1, &(registers->F));
	expected = (TestResult16){.result = 4096, .flags = 0x20};
	actual = (TestResult16){.result = registers->SP, .flags = registers->F};
	check_result_16(&expected, &actual);

	printf("Test: Throwing the carry flag and half carry flag\n");
	registers->SP = 65534;
	stack_pointer_add(&(registers->SP), 10, &(registers->F));
	expected = (TestResult16){.result = 8, .flags = 0x30};
	actual = (TestResult16){.result = registers->SP, .flags = registers->F};
	check_result_16(&expected, &actual);

	free(registers);
}

// Tests 16 bit increments
void test_increment_16() {
	TestResult16 expected;	// Holds the expected results
	TestResult16 actual;	// Holds the actual results 

	CPUState *registers = initialise_registers();

	printf("\nTesting increments of 16-bit values\n");
	printf("Incrementing 0 by 1. Value stored in the BC register\n");
	registers->BC = 0;
	increment_register_16(&(registers->BC));

	// Note: This operation does not alter the registers
	expected = (TestResult16){.result = 1, .flags = registers->F};
	actual = (TestResult16){.result = registers->BC, .flags = registers->F};
	check_result_16(&expected, &actual);

	// Now the other extreme
	printf("Incrementing 65535 by 1. Expected result is zero\n");
	registers->BC = 65535;
	increment_register_16(&(registers->BC));

	// Note: This operation does not alter the registers
	expected = (TestResult16){.result = 0, .flags = registers->F};
	actual = (TestResult16){.result = registers->BC, .flags = registers->F};
	check_result_16(&expected, &actual);
	free(registers);
}

// Testing our 16 bit decrements
void test_decrement_16() {
	TestResult16 expected;	// Holds the expected results
	TestResult16 actual;	// Holds the actual results 

	CPUState *registers = initialise_registers();

	printf("Testing decrements of 16-bit values\n");
	printf("Decrement 1 by 1. Value stored in the SP register\n");
	registers->SP = 1;
	decrement_register_16(&(registers->SP));

	// Note: This operation does not alter the registers
	expected = (TestResult16){.result = 0, .flags = registers->F};
	actual = (TestResult16){.result = registers->SP, .flags = registers->F};
	check_result_16(&expected, &actual);

	// Now the other extreme
	printf("Decrementing 0 by 1. Expected result is 65535\n");
	registers->SP = 0;
	decrement_register_16(&(registers->SP));

	// Note: This operation does not alter the registers
	expected = (TestResult16){.result = 65535, .flags = registers->F};
	actual = (TestResult16){.result = registers->SP, .flags = registers->F};
	check_result_16(&expected, &actual);
	free(registers);
}

// Test Left Rotate
void test_rotate_left() {
	TestResult expected;
	TestResult actual;

	CPUState *registers = initialise_registers();

	printf("\nTesting Left Rotates (8 Bit)\n");
	printf("Roating Value 0x80, 7 bit to carry\n");
	
	registers->F = 0x00;
	load_immediate_byte(&(registers->B), 0x80);
	rotate_register_left_carry_archive(&(registers->B), &(registers->F));

	expected = create_test_result(0x01, 0x10);
	actual = create_test_result(registers->B, registers->F);
	check_result(&expected, &actual);

	printf("Rotating Value 0x01, Expect Carry to be 0\n");
	registers->F = 0x00;
	load_immediate_byte(&(registers->B), 0x01);
	rotate_register_left_carry_archive(&(registers->B), &(registers->F));

	expected = create_test_result(0x02, 0x00);
	actual = create_test_result(registers->B, registers->F);
	check_result(&expected, &actual);

	printf("Rotate Indirect Value, Carry As Archive\n");
	registers->F = 0x00;
	memory_space[34] = 0x80;
	registers->HL = 34;
	
	rotate_indirect_left_carry_archive(&(registers->HL), &(registers->F));

	expected = create_test_result(0x01, 0x10);
	actual = create_test_result(memory_space[34], registers->F);
	check_result(&expected, &actual);

	printf("Rotating Indirect Value 0x01, Expect Carry to be 0\n");
	registers->F = 0x00;
	memory_space[34] = 0x01;
	registers->HL = 34;
	rotate_indirect_left_carry_archive(&(registers->HL), &(registers->F));

	expected = create_test_result(0x02, 0x00);
	actual = create_test_result(memory_space[34], registers->F);
	check_result(&expected, &actual);

	printf("Rotate 0x80 through the Carry Bit, Carry at 0\n");
	registers->F = 0x00;

	load_immediate_byte(&(registers->B), 0x80);
	rotate_register_left_through_carry(&(registers->B), &(registers->F));

	expected = create_test_result(0x00, 0x90);		// Zero and Carry are Set
	actual = create_test_result(registers->B, registers->F);
	check_result(&expected, &actual);

	printf("Rotate 0x80 through the Carry Bit, Carry at 1\n");
	registers->F = 0x10;

	load_immediate_byte(&(registers->B), 0x80);
	rotate_register_left_through_carry(&(registers->B), &(registers->F));

	expected = create_test_result(0x01, 0x10);
	actual = create_test_result(registers->B, registers->F);
	check_result(&expected, &actual);

	printf("Rotate 0x80 through the Carry Bit, Carry at 0 (Indirect)\n");
	registers->F = 0x00;
	memory_space[34] = 0x80;
	registers->HL = 34;

	rotate_indirect_left_through_carry(&(registers->HL), &(registers->F));

	expected = create_test_result(0x00, 0x90);		// Zero and Carry are set
	actual = create_test_result(memory_space[34], registers->F);
	check_result(&expected, &actual);

	printf("Rotate 0x80 through the Carry Bit, Carry at 1 (Indirect)\n");
	registers->F = 0x10;
	memory_space[34] = 0x80;
	registers->HL = 34;

	rotate_indirect_left_through_carry(&(registers->HL), &(registers->F));

	expected = create_test_result(0x01, 0x10);
	actual = create_test_result(memory_space[34], registers->F);
	check_result(&expected, &actual);
	free(registers);
}

// Test Right Rotate
void test_rotate_right() {
	TestResult expected;
	TestResult actual;

	CPUState *registers = initialise_registers();

	printf("\nTesting Right Rotates (8 Bit)\n");
	printf("Roating Value 0x11, 0 bit to carry\n");

	load_immediate_byte(&(registers->B), 0x11);
	rotate_register_right_carry_archive(&(registers->B), &(registers->F));

	expected = create_test_result(0x88, 0x10);
	actual = create_test_result(registers->B, registers->F);
	
	check_result(&expected, &actual);

	printf("Rotating Value 0x02, Expect Carry to be 0\n");
	
	load_immediate_byte(&(registers->B), 0x02);
	rotate_register_right_carry_archive(&(registers->B), &(registers->F));

	expected = create_test_result(0x01, 0x00);
	actual = create_test_result(registers->B, registers->F);
	check_result(&expected, &actual);

	printf("Rotate Indirect Value 0x11, Carry As 0 bit\n");
	
	memory_space[34] = 0x11;
	registers->HL = 34;
	
	rotate_indirect_right_carry_archive(&(registers->HL), &(registers->F));

	expected = create_test_result(0x88, 0x10);
	actual = create_test_result(memory_space[34], registers->F);
	check_result(&expected, &actual);

	printf("Rotating Indirect Value 0x02, Expect Carry to be 0\n");
	
	memory_space[34] = 0x02;
	registers->HL = 34;
	rotate_indirect_right_carry_archive(&(registers->HL), &(registers->F));

	expected = create_test_result(0x01, 0x00);
	actual = create_test_result(memory_space[34], registers->F);
	check_result(&expected, &actual);

	printf("Rotate 0x01 through the Carry Bit, Carry at 0\n");
	

	load_immediate_byte(&(registers->B), 0x01);
	rotate_register_right_through_carry(&(registers->B), &(registers->F));

	expected = create_test_result(0x00, 0x90);		// Zero and Carry are Set
	actual = create_test_result(registers->B, registers->F);
	check_result(&expected, &actual);

	printf("Rotate 0x01 through the Carry Bit, Carry at 1\n");
	registers->F = 0x10;

	load_immediate_byte(&(registers->B), 0x01);
	rotate_register_right_through_carry(&(registers->B), &(registers->F));

	expected = create_test_result(0x80, 0x10);
	actual = create_test_result(registers->B, registers->F);
	check_result(&expected, &actual);

	printf("Rotate 0x01 through the Carry Bit, Carry at 0 (Indirect)\n");
	registers->F = 0x00;
	memory_space[34] = 0x01;
	registers->HL = 34;

	rotate_indirect_right_through_carry(&(registers->HL), &(registers->F));

	expected = create_test_result(0x00, 0x90);		// Zero and Carry are set
	actual = create_test_result(memory_space[34], registers->F);
	check_result(&expected, &actual);

	printf("Rotate 0x01 through the Carry Bit, Carry at 1 (Indirect)\n");
	registers->F = 0x10;
	memory_space[34] = 0x01;
	registers->HL = 34;

	rotate_indirect_right_through_carry(&(registers->HL), &(registers->F));

	expected = create_test_result(0x80, 0x10);
	actual = create_test_result(memory_space[34], registers->F);
	check_result(&expected, &actual);
	free(registers);
}

void test_left_shifts() {
	TestResult expected;
	TestResult actual;

	CPUState *registers = initialise_registers();

	printf("\nTesting Left Shifts\n");

	printf("Shift 0x88 Left by One, Expect Carry Raised\n");
	load_immediate_byte(&(registers->A), 0x88);
	shift_register_left(&(registers->A), &(registers->F));
	expected = create_test_result(0x10, 0x10);
	actual = create_test_result(registers->A, registers->F);
	check_result(&expected, &actual);

	printf("Shift 0x80 Left by One, Expect Carry, Zero Raised\n");
	load_immediate_byte(&(registers->A), 0x80);
	shift_register_left(&(registers->A), &(registers->F));
	expected = create_test_result(0x00, 0x90);
	actual = create_test_result(registers->A, registers->F);
	check_result(&expected, &actual);

	printf("Shift 0x01 Left by One, Expect No Flags Raised\n");
	load_immediate_byte(&(registers->A), 0x01);
	shift_register_left(&(registers->A), &(registers->F));
	expected = create_test_result(0x02, 0x00);
	actual = create_test_result(registers->A, registers->F);
	check_result(&expected, &actual);

	printf("Shift 0x80 Left By One, Expect Carry, Zero Raised (Indirect)\n");
	memory_space[35] = 0x80;
	registers->HL = 35;
	shift_indirect_left(&(registers->HL), &(registers->F));
	expected = create_test_result(0x00, 0x90);
	actual = create_test_result(memory_space[35], registers->F);
	check_result(&expected, &actual);

	printf("Shift 0x01 Left By One, Expect No Flags Raised (Indirect)\n");
	memory_space[35] = 0x01;
	registers->HL = 35;
	shift_indirect_left(&(registers->HL), &(registers->F));
	expected = create_test_result(0x02, 0x00);
	actual = create_test_result(memory_space[35], registers->F);
	check_result(&expected, &actual);

	free(registers);
}

void test_right_shifts() {
	TestResult expected;
	TestResult actual;

	CPUState *registers = initialise_registers();
	
	printf("\nTesting Right Shifts (Arithmetic)\n");

	printf("Shift 0x02 Right (Arithmetic), Expect No Flags Set\n");
	load_immediate_byte(&(registers->A), 0x02);
	arithmetic_shift_register_right(&(registers->A), &(registers->F));
	expected = create_test_result(0x01, 0x00);
	actual = create_test_result(registers->A, registers->F);
	check_result(&(expected), &(actual));

	printf("Shift 0x01 Right (Arithmetic), Expect Zero, Carry Flags Set\n");
	load_immediate_byte(&(registers->A), 0x01);
	arithmetic_shift_register_right(&(registers->A), &(registers->F));
	expected = create_test_result(0x00, 0x90);
	actual = create_test_result(registers->A, registers->F);
	check_result(&(expected), &(actual));

	printf("Shift 0xF0 Right (Arithmetic). MSB to propagate\n");
	load_immediate_byte(&(registers->A), 0xF0);
	arithmetic_shift_register_right(&(registers->A), &(registers->F));
	expected = create_test_result(0xF8, 0x00);
	actual = create_test_result(registers->A, registers->F);
	check_result(&(expected), &(actual));

	printf("Shift 0x02 Right (Arithmetic), Expect No Flags Set (Indirect)\n");
	memory_space[23] = 0x02;
	registers->HL = 23;
	arithmetic_shift_indirect_right(&(registers->HL), &(registers->F));
	expected = create_test_result(0x01, 0x00);
	actual = create_test_result(memory_space[23], registers->F);
	check_result(&(expected), &(actual));

	printf("Shift 0x01 Right (Arithmetic), Expect Zero, Carry Flags Set (Indirect)\n");
	memory_space[23] = 0x01;
	registers->HL = 23;
	arithmetic_shift_indirect_right(&(registers->HL), &(registers->F));
	expected = create_test_result(0x00, 0x90);
	actual = create_test_result(memory_space[23], registers->F);
	check_result(&(expected), &(actual));

	printf("Shift 0xF0 Right (Arithmetic), Expect MSB To Propagate (Indirect)\n");
	memory_space[23] = 0xF0;
	registers->HL = 23;
	arithmetic_shift_indirect_right(&(registers->HL), &(registers->F));
	expected = create_test_result(0xF8, 0x00);
	actual = create_test_result(memory_space[23], registers->F);
	check_result(&(expected), &(actual));

	printf("\nTesting Right Shifts (Logical)\n");

	printf("Shift 0x02 Right (Logical), Expect No Flags Set\n");
	load_immediate_byte(&(registers->A), 0x02);
	logical_shift_register_right(&(registers->A), &(registers->F));
	expected = create_test_result(0x01, 0x00);
	actual = create_test_result(registers->A, registers->F);
	check_result(&(expected), &(actual));

	printf("Shift 0x01 Right (Logical), Expect Zero, Carry Flags Set\n");
	load_immediate_byte(&(registers->A), 0x01);
	logical_shift_register_right(&(registers->A), &(registers->F));
	expected = create_test_result(0x00, 0x90);
	actual = create_test_result(registers->A, registers->F);
	check_result(&(expected), &(actual));

	printf("Shift 0xF0 Right (Logical). MSB will not propgagate\n");
	load_immediate_byte(&(registers->A), 0xF0);
	logical_shift_register_right(&(registers->A), &(registers->F));
	expected = create_test_result(0x78, 0x00);
	actual = create_test_result(registers->A, registers->F);
	check_result(&(expected), &(actual));

	printf("Shift 0x02 Right (Logical), Expect No Flags Set (Indirect)\n");
	memory_space[23] = 0x02;
	registers->HL = 23;
	logical_shift_indirect_right(&(registers->HL), &(registers->F));
	expected = create_test_result(0x01, 0x00);
	actual = create_test_result(memory_space[23], registers->F);
	check_result(&(expected), &(actual));

	printf("Shift 0x01 Right (Logical), Expect Zero, Carry Flags Set (Indirect)\n");
	memory_space[23] = 0x01;
	registers->HL = 23;
	logical_shift_indirect_right(&(registers->HL), &(registers->F));
	expected = create_test_result(0x00, 0x90);
	actual = create_test_result(memory_space[23], registers->F);
	check_result(&(expected), &(actual));

	printf("Shift 0xF0 Right (Logical), MSB will not propagate (Indirect)\n");
	memory_space[23] = 0xF0;
	registers->HL = 23;
	logical_shift_indirect_right(&(registers->HL), &(registers->F));
	expected = create_test_result(0x78, 0x00);
	actual = create_test_result(memory_space[23], registers->F);
	check_result(&(expected), &(actual));

	free(registers);

}

// Testing some nibble swaps
void test_nibble_swap() {
	TestResult expected;	// Holds the expected results
	TestResult actual;	// Holds the actual results 

	CPUState *registers = initialise_registers();

	// Testing with 0
	printf("\nTesting Nibble Swaps\n");
	printf("Swaping a Register Value 0x00\n");
	load_immediate_byte(&(registers->A), 0x00);
	swap_nibble_register(&(registers->A), &(registers->F));
	expected = (TestResult){.result = 0x00, .flags = 0x80};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Swaping a Register Value 0x67\n");
	load_immediate_byte(&(registers->B), 0x67);
	swap_nibble_register(&(registers->B), &(registers->F));
	expected = (TestResult){.result = 0x76, .flags = 0x00};
	actual = (TestResult){.result = registers->B, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Testing an indirect value\n");
	memory_space[43] = 0xA0;
	registers->HL = 43;
	swap_nibble_indirect(&(registers->HL), &(registers->F));
	expected = (TestResult){.result = 0x0A, .flags = 0x00};
	actual = (TestResult){.result = memory_space[43], .flags = registers->F};
	check_result(&expected, &actual);

	free(registers);
}

// Testing the instructions that.. well test our bits.
void test_test_bit() {
	TestResult expected;
	TestResult actual;

	CPUState *registers = initialise_registers();

	printf("\nTest Bit Direct\n");

	// Note: All we are concerened about are the flag values
	printf("Register A is 0x05, test bit 7. Not set so Zero is raised\n");
	load_immediate_byte(&(registers->A), 0x05);
	test_bit_register(&(registers->A), 7, &(registers->F));
	expected = create_test_result(0x00, 0xA0);			// Half Carry is always set
	actual = create_test_result(0x00, registers->F);
	check_result(&expected, &actual);

	printf("Register A is 0x05, test bit 1. Not set so Zero is raised\n");
	load_immediate_byte(&(registers->A), 0x05);
	test_bit_register(&(registers->A), 7, &(registers->F));
	expected = create_test_result(0x00, 0xA0);			// Half Carry is always set
	actual = create_test_result(0x00, registers->F);
	check_result(&expected, &actual);

	printf("Register A is 0x05, test bit 0 expect set, Zero not raised\n");
	load_immediate_byte(&(registers->A), 0x05);
	test_bit_register(&(registers->A), 0, &(registers->F));
	expected = create_test_result(0x00, 0x20);			// Half Carry is always set
	actual = create_test_result(0x00, registers->F);
	check_result(&expected, &actual);

	printf("Register A is 0x05, test bit 2 expect set, Zero not raised\n");
	load_immediate_byte(&(registers->A), 0x05);
	test_bit_register(&(registers->A), 2, &(registers->F));
	expected = create_test_result(0x00, 0x20);			// Half Carry is always set
	actual = create_test_result(0x00, registers->F);
	check_result(&expected, &actual);

	printf("\nTest Bit Indirect\n");

	printf("Test 0x05, bit 7. Not set so expect Zero raised.");
	memory_space[45] = 0x05;
	registers->HL = 45;
	test_bit_indirect(&(registers->HL), 7, &(registers->F));
	expected = create_test_result(0x00, 0xA0);			// Half Carry is always set
	actual = create_test_result(0x00, registers->F);
	check_result(&expected, &actual);

	printf("Test 0x05, bit 1. Not set so expect Zero raised.");
	memory_space[45] = 0x05;
	registers->HL = 45;
	test_bit_indirect(&(registers->HL), 1, &(registers->F));
	expected = create_test_result(0x00, 0xA0);			// Half Carry is always set
	actual = create_test_result(0x00, registers->F);
	check_result(&expected, &actual);

	printf("Test 0x05, bit 0. Set so expect Zero not to be raised.");
	memory_space[45] = 0x05;
	registers->HL = 45;
	test_bit_indirect(&(registers->HL), 0, &(registers->F));
	expected = create_test_result(0x00, 0x20);			// Half Carry is always set
	actual = create_test_result(0x00, registers->F);
	check_result(&expected, &actual);	

	printf("Test 0x05, bit 2. Set so expect Zero not to be raised.");
	memory_space[45] = 0x05;
	registers->HL = 45;
	test_bit_indirect(&(registers->HL), 2, &(registers->F));
	expected = create_test_result(0x00, 0x20);			// Half Carry is always set
	actual = create_test_result(0x00, registers->F);
	check_result(&expected, &actual);	
	
}

// Testing Complement Operations
void test_complements() {
	TestResult expected;	// Holds the expected results
	TestResult actual;		// Holds the actual results 

	CPUState *registers = initialise_registers();

	printf("\nTesting Complement Operations:\n");
	printf("Test: Complementing Accumulator\n");
	load_immediate_byte(&(registers->A), 0xAF);
	complement_accumulator(&(registers->A), &(registers->F));
	expected = (TestResult){.result = 0x50, .flags = 0x60};
	actual = (TestResult){.result = registers->A, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Test: Complementing Carry Flag\n");
	registers->F = 0x10;
	complement_carry_flag(&(registers->F));
	expected = (TestResult){.result = 0x00, .flags = 0x00};
	actual = (TestResult){.result = 0x00, .flags = registers->F};
	check_result(&expected, &actual);

	printf("Test: Setting Carry Flag, Complement Carry Flag\n");
	registers->F = 0x00;
	complement_carry_flag(&(registers->F));
	expected = (TestResult){.result = 0x00, .flags = 0x10};
	actual = (TestResult){.result = 0x00, .flags = registers->F};
	check_result(&expected, &actual);

	free(registers);
}

// Utilities to check the result of our tests. 

/**
 * /brief Checks the result of an ALU operation against a test case.
 * 
 * Checks an ALU operation against a test case to ensure both the result
 * and status flags are correctly set. If the test passes, "SUCCESS"
 * will br printed. Should it fail, a more verbose perint out will be 
 * printed to console.
 *
 * @param: expected: Expected Output
 * @param: actual: The actual result of the operation;
 */
void check_result(TestResult *expected, TestResult *actual) {
	char test_success;

	test_success = expected->result == actual->result;	// test_success is 1 if true
	test_success &= expected->flags == actual->flags;	// 1 & X == X. So to stay at 1 this must be equivalent

	// If test_success is 
	if (!test_success) {
		++failures;
		printf("\tFAILURE :_(\n");
		printf("\tResult:\n\t\tExpected: %X\n\t\tActual: %X\n\n", 
			expected->result, actual->result);
		printf("\tFlags:\n\t\tExpected: %X\n\t\tActual: %X\n\n",
			expected->flags, actual->flags);
	}
	else {
		++successes;
		printf("\tSUCCESS!\n\n");
	}
}

/**
 * /brief Constructs a test result struct.
 * 
 * Creates a TestResult struct with the desired result and flags
 * fields
 * 
 * @param result: The result of a test operation, expected or actual
 * @param flags: The flags after a test result, expected or actual
 * 
 * @returns A well formed test result struct
 */
TestResult create_test_result(Register8 result, Register8 flags) {
	return (TestResult){.result = result, .flags = flags};
}

/**
 * /brief Checks the result of a 16 bit alu operation
 *
 * Checks both the result and flags of a 16 bit ALU operation to ensure
 * both the result and status flags have been correctly set. 
 * If the test passes, "SUCCESS" will be printed. Should it fail, 
 * a more verbose perint out will be printed to console.
 * @param: expected: Expected Output
 * @param: actual: The actual result of the operation;
 */
void check_result_16(TestResult16 *expected, TestResult16 *actual) {
	char test_success;

	test_success = expected->result == actual->result;	// test_success is 1 if true
	test_success &= expected->flags == actual->flags;	// 1 & X == X. So to stay at 1 this must be equivalent

	// If test_success is 
	if (!test_success) {
		++failures;
		printf("\tFAILURE :_(\n");
		printf("\tResult:\n\t\tExpected: %X\n\t\tActual: %X\n\n", 
			expected->result, actual->result);
		printf("\tFlags:\n\t\tExpected: %X\n\t\tActual: %X\n\n",
			expected->flags, actual->flags);
	}
	else {
		++successes;
		printf("\tSUCCESS!\n\n");
	}
}




