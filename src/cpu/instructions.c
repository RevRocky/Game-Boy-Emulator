/** 
 * This here module contains a motherlode of instructions to be executed in emulating the 
 * Z80 processor in a GameBoy. Very important to note: there is not a 1-to-1 relationship
 * of instruction to Opcode as the Gameboy will often use a seperate Opcode for something 
 * like:
 *
 *		add B
 * and 
 *
 * 		add C
 *
 * Where as in this implimentation we simply would have one function with varying arguments.
 * To ease development, as much as possible I will try to annotate each function with the 
 * op codes that would result in such a function being called. 
 *
 * Other important note: Unless specified otherwise, it will be assumed that pointers to 
 * register values are being passed in. this will allow for values to be manipulated in place 
 * This decision does lead to the implimentation of seperate functions for working with 
 * immediate values and those already stored in registers. 
 *
 * Note: The following instructions are not implemented in this file:
 * 		NOP, HALT, STOP, DI, EI, 
 *
 * Authors: Rocky Petkov
 */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "../util.h"
#include "instructions.h"
#include "../memory/memory.h"

// Here's some functions we don't want to be visible!
// Flag Management
Register8 set_flags_add(Register8 result, Register8 old_accumulator);
Register8 set_flags_sub(Register8 result, Register8 old_accumulator);

/*** 8-BIT LOADS  ***/

/*
 * /brief Standard 8 bit load with immediate value.
 *
 * This function implements the rudimentary loading of an immediate
 * value into any one of the 8 bit registers.
 * 
 * Note: There is nothing preventing this function being used to load
 * to the flags register, thus it will be incumbent upon the calling
 * environment to ensure that this is adhered to.
 *
 * This instruction implements opcodes:
 * 		0x06, 0x0E, 0x16, 0x1E, 0x26, 0x2E, Ox3E
 * 
 * @param: destination: The register we will load a value into.
 * @param: value: An 8 bit immediate value which we will load into
 * 			said register
 */
void load_immediate_byte(Register8 *destination, unsigned char value) {
	*destination = value;						// Riveting stuff. 
}

/**
 * /brief Loads the value stored in one register into another.
 * 
 * A standard register load. The value in source is loaded into destination.
 * 
 * This function implements the following opcodes:
 * 		7F, 78, 79, 7A, 7B, 7C, 7D, 7F
 * 		40, 41, 42, 42, 44, 45, 47
 * 		48, 49, 4A, 4B, 4C, 4D, 4F
 * 		50, 51, 52, 53, 54, 55, 57
 * 		58, 59, 5A, 5B, 5C, 5D, 5F
 * 		60, 61, 62, 63, 64, 65, 67
 * 		68, 69, 6A, 6B, 6C, 6D, 6F
 * 
 * @param destination: Pointer to the destination register
 * @param source: Pointer to the source register
 */
void load_register(Register8 *destination, Register8 *source) {
	*destination = *source;
}

/**
 * /brief A register load where the destination is a memory address contained within 
 *		 the address register.
 *
 * A register load where the destination is a location in memory accessed through indirect
 * addressing. 
 * 
 * TODO: Consider renaming this function to something clearer.
 * 
 * This function implements the following opcodes:
 * 		70, 71, 72, 73, 74, 75, 02, 12, 77
 * 
* @param adress_register: The register that contains the address of our operand. 
* 		Unlike a lot of other instructions which utilise indirect addressing in the 
* 		Z80GB, it is legal to supply some of the other 16 bit register pairs (BC, DE, HL),
* 		to this load. While it is possible to do this, it is only legal when the destination
* 		register is the Accumulator. 
 * @param source: The register containing the value we will write to memory
 */
void load_register_indirect_destination(Register16 *address_register, Register8 *source) {
	write_byte(*address_register, *source);
}

/**
 * /brief Reads the value stored at the memory address referenced in the address register
 * 		 into the supplied destination register
 * 
 * Reads the value stored at a given memory address (referenced in the address register)
 * 		into the supplied destination register. 
 * 
 * This function implements the following opcodes: 
 * 		7E, 46, 4E, 56, 5E, 66, 6E, 0A, 1A
 * 
 *@param destination: The 8-bit register that will be the destination of the load.
* @param adress_register: The register that contains the address of our operand. 
* 		Unlike a lot of other instructions which utilise indirect addressing in the 
* 		Z80GB, it is legal to supply some of the other 16 bit register pairs (BC, DE, HL),
* 		to this load. While it is possible to do this, it is only legal when the destination
* 		register is the Accumulator. 
 */
void load_register_indirect_source(Register8 *destination, Register16 *address_register) {
	unsigned char value_at_address = read_byte(*address_register);
	*destination = value_at_address;
}

/**
 * /brief Loads the accumulator with the value at the supplied memory address
 * 
 * Loads the accumulator with a value stored at the supplied memory address.
 * 
 * This function implements the following opcodes: 
 * 		3E
 * 
 * @param accumulator: Pointer to the accumulator register.
 * @param memory_address: The memory address of the value we are loading into the 
 * 		accumulator. We treat this value in a similar way we do indirect addressing.
 */
void load_accumulator_from_address(Register8 *accumultator, unsigned short memory_address) {
	*accumultator = read_byte(memory_address);
}

/**
 * /brief Writes the value stored in the accumulator to a specified memory address
 * 
 * Writes the value in the accumulator to the supplied memory address
 * 
 * This function implements the following opcodes:
 * 		EA
 * 
 * @param memory_address: The memory address of the value we wish to store the value of the 
 * 		accumulator in.
 * @param accumulator: Pointer to the accumulator
 */
void write_accumulator_to_address(unsigned short memory_address, Register8 *accumulator) {
	write_byte(memory_address, *accumulator);
}

/**
 * /brief Loads accumulator with value at address stored in address register. Decrements the address register.
 * 
 * Loads the accumulator with a value at the address in the address register. The address register is then decremented.
 * 
 * This function implements the following opcodes: 
 * 		3A
 * 
 * @param accumulator: Pointer to the accumulator.
 * @param address_register: Pointer to the (HL) address register. Decremented as part of the operation.
 */
void load_accumulator_decrement_address_register(Register8 *accumulator, Register16 *address_register) {
	load_register_indirect_source(accumulator, address_register);
	--address_register;
}

/**
 * /brief Loads accumulator with value at address stored in address register. Increments the address register.
 * 
 * Loads the accumulator with a value at the address in the address register. The address register is then incremented
 * 
 * This function implements the following opcodes:
 * 		2A
 * 
 * @param accumulator: Pointer to the accumulator.
 * @param address_register: Pointer to the (HL) address register. Incremented after load.
 */
void load_accumulator_increment_address_register(Register8 *accumulator, Register16 *address_register) {
	load_register_indirect_source(accumulator, address_register);
	++address_register;
}

/**
 * /brief Saves the accumulator to the memory address referenced in the address register. Decrement address register.
 * 
 * Saves the accumulator to the memory address referenced in the address register. The address register is then decremented.
 * 
 * This function implements the following opcodes:
 * 		32
 * 
 * @param address_register: Pointer to the address register. Decremented after load.
 * @param accumulator: Pointer to the accumulator
 */
void write_accumulator_decrement_address_register(Register16 *address_register, Register8 *accumulator) {
	load_register_indirect_destination(address_register, accumulator);
	--address_register;
}

/**
 * /brief Saves the accumulator to the memory address referenced in the address register. Increment address register.
 * 
 * Saves the accumulator to the memory address referenced in the address register. The address register is then incremented.
 * 
 * This function implements the following opcodes: 
 * 		22
 * 
 * @param address_register: Pointer to the address register. Incremented after load.
 * @param accumulator: Pointer to the accumulator
 */
void write_accumulator_increment_address_register(Register16 *address_register, Register8 *accumulator) {
	load_register_indirect_destination(address_register, accumulator);
	++address_register;
}

/**
 * /brief Loads accumulator with value in the C'th IO port. Where C is the value in
 * 		the offset register.
 * 
 * Used to load values from an IO port into the accumulator. The IO port we read from is 
 * the C'th IO port where C is the value stored within the "C" register/
 * 
 * This function implements the following opcodes: 
 * 		F2
 * 
 * @param accumulator: Pointer to the accumulator.
 * @param offset_register: Pointer to the register with the value used as our offset. 
 * 		This will only be called with the offset_register being Register C.
 */
void load_from_io_port_c(Register8 *accumulator, Register8 *offset_register) {
	unsigned short source_address = *offset_register + IO_PORT_MEMORY_BASE;
	load_accumulator_from_address(accumulator, source_address);
}

/**
 * /brief Writes the accumulator to the specified IO port.
 * 
 * Used to write values to an IO port. The IO port we write to is 
 * the C'th IO port where C is the value stored within the "C" register/
 * 
 * This function implements the following opcodes: 
 * 		E2
 * 
 * @param offset_register: Pointer to the register with the value used as our offset. 
 * 		This will only be called with the offset_register being Register C.
 * @param accumulator: Pointer to the accumulator.
 */
void write_to_io_port_c(Register8 *offset_register, Register8 *accumulator) {
	unsigned short source_address = *offset_register + IO_PORT_MEMORY_BASE;
	write_accumulator_to_address(source_address, accumulator);
}

/**
 * /brief Reads from the N'th IO port.
 * 
 * Used to load values from an IO port into the accumulator. The IO port read from is 
 * the N'th IO port where N is an 8 bit value representing the offset (in the memory address space)
 * from 0xFF00.
 * 
 * This function implements the following opcodes: 
 * 		E0
 * 
 * @param accumulator: Pointer to the accumulator register.
 * @param offset: Specifies the IO port we are reading form
 */
void load_from_io_port_n(Register8 *accumulator, unsigned char offset) {
	unsigned short source_address = offset + IO_PORT_MEMORY_BASE;
	load_accumulator_from_address(accumulator, source_address);
}

/**
 * /brief Wries the N'th IO port.
 * 
 * Used to write values to an IO port into the accumulator. The IO port written to
 * the N'th IO port where N is an 8 bit value representing the offset (in the memory address space)
 * from 0xFF00.
 * 
 * This function implements the following opcodes
 * 		F0
 * 
 * @param offset: Specifies the IO port we are writing to.
 * @param accumulator: Pointer to the accumulator register.
 */
void write_to_io_port_n(unsigned char offset, Register8 *accumulator) {
	unsigned short source_address = offset + IO_PORT_MEMORY_BASE;
	write_accumulator_to_address(source_address, accumulator);
}


/*** 16-BIT LOADS ***/

/**
 * /brief Standard 16 bit load to register.
 * 
 * Loads a 16 bit value to a 16 bit register pair.
 * 
 * This function implements the following opcodes:
 * 		01, 11, 21, 31
 * 
 * @param destination: Pointer to the destination register
 * @param value: A 16 bit immediate value
 */
void load_immediate_short(Register16 *destination, unsigned short value) {
	*destination = value;
}

/**
 * /brief Loads the stack pointer with the value in the source register
 * 
 * Loads the stack pointer with the value in the source register.
 * This function is only legal when the source register is the HL register.
 * 
 * Note: unlike most uses of the HL register, this does not use indirect 
 * addressing.
 * 
 * This function implements the following opcodes: 
 *		F9
 * 
 * @param stack_pointer: Pointer to the stack... erm... pointer
 * @param source_register: Pointer to register we are loading from. Only legal if 
 * 		the HL register
 */
void load_stack_pointer(Register16 *stack_pointer, Register16 *source_register) {
	*stack_pointer = *source_register;
}

/**
 * /brief Adds Offset to the current value of the stack pointer
 * 
 * Adds the value of offset to the current value of the stack pointer
 * 
 * This function implements the following opcodes: 
 *		F8
 *
 * @param stack_pointer: Pointer to the... stack pointer
 * @param offset: The amount we are adding/subtracting (if negative) to the stack pointer
 * @param flags: Pointer to the flags register. Due to ambigious specifications we assume that
 * 		we are to reset the flags register.
 */
void load_stack_pointer_offset(Register16 *stack_pointer, short offset, Register8 *flags) {
	*stack_pointer + offset;
	*flags = 0x00;
}

/**
 * /brief Writes the stack pointer to the supplied address
 * 
 * Writes the stack pointer to an address. Since the stack pointer is a 16 bit value 
 * and our typical memory values are 8 bits, we will write the value in  an 
 * ascending manner. This means that the upper byte will be placed at the address
 * "address". We will then increment the address and then write the lower byte.
 * 
 * This function implements the following opcodes: 
 * 		08
 * 
 * @param stack_pointer: Pointer to the stack pointer.
 * @param address: The address we will write the stack pointer to. Technically the 
 * 		lower byte of the stack pointer will be written here.
 */
void write_stack_pointer_to_address(Register16 *stack_pointer, unsigned short address) {
	unsigned char lower_byte = (unsigned char) *stack_pointer & 0x0F;
	unsigned char upper_byte = (unsigned char) (*stack_pointer >> 4) & 0x0F;

	// Assuming memory, like most things is going to be little endian...
	write_byte(address, upper_byte);
	write_byte(++address, lower_byte);
}


/**
 * /brief Pushes 16 bit value in supplied register to stack
 * 
 * Pushes the value in the source register to the stack. The stack pointer
 * is then decremented twice.
 * 
 * This function implements the following opcodes:
 * 		F5, C5, D5, E5
 * 
 * @param stack_pointer: Pointer to the... stack pointer. Decremented twice in the 
 * 		operation.
 * @param source_register: The register we are pushing onto the stack
 */
void push(Register16 *stack_pointer, Register16 *source_register) {
	unsigned char lower_byte = (unsigned char) *source_register & 0x0F;
	unsigned char upper_byte = (unsigned char) (*source_register >> 4) & 0x0F;

	write_byte((*stack_pointer)--, lower_byte);
	write_byte((*stack_pointer)--, upper_byte);
}

/**
 * /brief Pops a value from the stack into a 16 bit register pair.
 * 
 * Pops the value at the "top" of the stack into the supplied 16 bit destination 
 * register. The SP is incremented twice.
 * 
 * This function implements the following opcodes:
 * 		F1, C1, D1, E1
 *
 * @param stack_pointer: Pointer to the stack pointer. Incremented twice during 
 * 		the operation
 * @param destination_register: The 16 bit register where the value will be stored.
 */
void pop(Register16 *stack_pointer, Register16 *destination_register) {
	// We store the old PC s.t. the MSNibble will be read first.
	unsigned short new_destination_value = read_byte((*stack_pointer)++) << 4; 	// Stack is descending
	new_destination_value = read_byte((*stack_pointer)++) | new_destination_value;	// Read LSB, mask with MSB.

	*destination_register = new_destination_value;
}

/*** 8-BIT ARITHMETIC / LOGICAL ***/

// Arithmetic Operations // 

/**
 * /brief 8 bit add To be used when the operand is a register
 *
 * The standard 8 bit add. All changes to values are done in place.
 * This function will be used to implement the following opcodes:
 * 		0x87, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85
 * 
 * @param accumulator: Pointer to the accumulator register: A.
 * @param other_register: Pointer to the other register we wish to add to the 
 *		accumulator.
 * @param flags: Pointer to the status flags register. 
 */
void add_register(Register8 *accumulator, Register8 *other_register, Register8 *flags) {
	unsigned char result = *accumulator + *other_register;		// I opt not to immediately store to make the rest of this code cleaner to follow!

	// Set flags and accumulator and be done
	*flags = set_flags_add(result, *accumulator);
	*accumulator = result;
}

/**
 * /brief 8 bit add. To be used when the operand is an immediate 8 bit value.
 *
 * The standard 8 bit add. All changes to values are done in place.
 * This function will be used to implement the following opcodes:
 * 		0xC6. 
 *
 * Note: the second operand will be expected to be found in the next byte in the 
 *		game's source code.
 * 
 * @param accumulator: Pointer to the accumulator register: A.
 * @param value: 8-bit value we wish to add to the accumulator. Unsigned
 * @param flags: Pointer to the status flags register. 
 */
void add_immediate(Register8 *accumulator, unsigned char value, Register8 *flags) {
	unsigned char result = *accumulator + value;	// Like above, we are not doing immediate storage

	// Set flags and accumulator and be done
	*flags = set_flags_add(result, *accumulator);
	*accumulator = result;
}

/**
 * /brief An 8 bit add that uses indirect addressing to fetch a value and then add it
 *
 * A standard 8 bit add, this time with a twist. The value passed in is simply an address
 * in a register and we have to fetch the actual value from memory and then perform the 
 * add. 
 * This function is used to impliment the following opcode:
 *		0x86
 *
 * @param accumulator: Pointer to the accumulator register: A.
 * @param address_register: Pointer to the "16 bit register" HL. This will tell us where 
 * 		to find our operand in memory.
 * @param flags: Pointer to the status flags register.
 */
void add_indirect(Register8 *accumulator, Register16 *address_register, Register8 *flags) {
	unsigned char value_at_address = read_byte(*address_register);
	unsigned char result = *accumulator + value_at_address;

	// Set flags and accumulator and be done
	*flags = set_flags_add(result, *accumulator);
	*accumulator = result;
}

/**
 * /brief 8 bit add. This time with carry To be used when the operand is a register
 *
 * An 8 bit add with carry. The result is A -> A + R2 + carry.
 * This function will be used to implement the following opcodes:
 * 		0x8F, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D
 * 
 * @param accumulator: Pointer to the accumulator register: A.
 * @param other_register: Pointer to the other register we wish to add to the 
 *		accumulator.
 * @param flags: Pointer to the status flags register. 
 */
void add_register_with_carry(Register8 *accumulator, Register8 *other_register, Register8 *flags) {
	unsigned char carry = (*flags & 0x10) >> 4;
	unsigned char result = carry + *accumulator + *other_register;

	// Set flags and accumulator and be done
	*flags = set_flags_add(result, *accumulator);
	*accumulator = result;
}

/**
 * /brief 8 bit add. This time with carry. To be used when the operand is an intermediate value
 *
 * An 8 bit add with carry, where we use an immediate result.
 * The result is A -> A + Value + carry.
 * This function will be used to implement the following opcodes:
 * 		0xCE
 * 
 * Note: the second operand will be expected to be found in the next byte in the 
 *		game's source code.
 * 
 * @param accumulator: Pointer to the accumulator register: A.
 * @param value: The value we wish to add to the accumulator 
 * @param flags: Pointer to the status flags register. 
 */
void add_immediate_with_carry(Register8 *accumulator, unsigned char value, Register8 *flags) {
	unsigned char carry = (*flags & 0x10) >> 4;
	unsigned char result = carry + *accumulator + value;

	// Set flags and accumulator and be done
	*flags = set_flags_add(result, *accumulator);
	*accumulator = result;
}

/**
 * /brief 8 bit add with indirect addressing. This time with carry. 
 *
 * A standard 8 bit add, this time with a twist. The value passed in is simply an address
 * in a register and we have to fetch the actual value from memory and then perform the 
 * add. 
 * The result is A -> A + *address_register + carry.
 * This function will be used to implement the following opcodes:
 * 		0x8e
 * 
 * @param accumulator: Pointer to the accumulator register: A.
 * @param address_register: Pointer to 16 bit register. Value in this register will be stored used in calculation 
 * @param flags: Pointer to the status flags register. 
 */
void add_indirect_with_carry(Register8 *accumulator, Register16 *address_register, Register8 *flags) {
	unsigned char carry = (*flags & 0x10) >> 4;
	unsigned char value_at_address = read_byte(*address_register);
	unsigned char result = carry + *accumulator + value_at_address;

	// Set flags and accumulator and be done
	*flags = set_flags_add(result, *accumulator);
	*accumulator = result;
}

/**
 * /brief Performs a standard subtraction operation on an operand stored in a register
 * A standard 8 bit subtract where the operand is stored within a register.
 * The result is simply A = A - *other_register.
 *
 * As with everything else, the operartion is done in place.
 * 
 * This function implements the opcodes:
 *		0x97, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95
 *
 * @param: accumulator: A pointer to the accumulator register
 * @param: other_register: Pointer to the other register being used
 * @param: flags: Pointer to the flags register
 */
void subtract_register(Register8 *accumulator, Register8 *other_register, Register8 *flags) {
	unsigned char result = *accumulator - *other_register;	// Like above, we are not doing immediate storage

	// Set flags and accumulator and be done
	*flags = set_flags_sub(result, *accumulator);
	*accumulator = result;
}

/**
 * /brief 8 bit subtract. To be used when the operand is an immediate 8 bit value.
 *
 * The standard 8 bit subtract. All changes to values are done in place.
 * This function will be used to implement the following opcodes:
 * 		0xD6. 
 *
 * Note: the second operand will be expected to be found in the next byte in the 
 *		game's source code.
 * 
 * @param accumulator: Pointer to the accumulator register: A.
 * @param value: 8-bit value we wish to subtract from the accumulator. Unsigned
 * @param flags: Pointer to the status flags register. 
 */
void subtract_immediate(Register8 *accumulator, unsigned char value, Register8 *flags) {
	unsigned char result = *accumulator - value;	// Like above, we are not doing immediate storage

	// Set flags and accumulator and be done
	*flags = set_flags_sub(result, *accumulator);
	*accumulator = result;
}

/**
 * /brief An 8 bit subtract that uses indirect addressing to fetch a value and then subtract it
 *
 * A standard 8 bit subtract, this time with a twist. The value passed in is simply an address
 * in a register and we have to fetch the actual value from memory and then perform the 
 * subtract. 
 * This function is used to impliment the following opcode:
 *		0x96
 *
 * @param accumulator: Pointer to the accumulator register: A.
 * @param address_register: Pointer to the "16 bit register" HL. This will tell us where 
 * 		to find our operand in memory.
 * @param flags: Pointer to the status flags register.
 */
void subtract_indirect(Register8 *accumulator, Register16 *address_register, Register8 *flags) {
	unsigned char value_at_address = read_byte(*address_register);
	unsigned char result = *accumulator - value_at_address;

	// Set flags and accumulator and be done
	*flags = set_flags_sub(result, *accumulator);
	*accumulator = result;
}

/**
 * /brief 8 bit subtract. This time with carry To be used when the operand is a register
 *
 * An 8 bit subtract with carry. The result is A -> A - R2 - carry.
 * This function will be used to implement the following opcodes:
 * 		0x9F, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D
 * 
 * @param accumulator: Pointer to the accumulator register: A.
 * @param other_register: Pointer to the other register we wish to subtract ffom the 
 *		accumulator.
 * @param flags: Pointer to the status flags register. 
 */
void subtract_register_with_carry(Register8 *accumulator, Register8 *other_register, Register8 *flags) {
	unsigned char carry = (*flags & 0x10) >> 4;
	unsigned char result = *accumulator - *other_register - carry;

	// Set flags and accumulator and be done
	*flags = set_flags_sub(result, *accumulator);
	*accumulator = result;
}

/**
 * /brief 8 bit subtract. This time with carry. To be used when the operand is an intermediate value
 *
 * An 8 bit subtract with carry, where we use an immediate result.
 * The result is A -> A - Value - carry.
 * This function will be used to implement the following opcodes:
 * 		0x96
 * 
 * Note: the second operand will be expected to be found in the next byte in the 
 *		game's source code.
 * 
 * @param accumulator: Pointer to the accumulator register: A.
 * @param value: The value we wish to subtract to the accumulator 
 * @param flags: Pointer to the status flags register. 
 */
void subtract_immediate_with_carry(Register8 *accumulator, unsigned char value, Register8 *flags) {
	unsigned char carry = (*flags & 0x10) >> 4;
	unsigned char result = *accumulator - value - carry;

	// Set flags and accumulator and be done
	*flags = set_flags_sub(result, *accumulator);
	*accumulator = result;
}

/**
 * /brief 8 bit subtract with indirect addressing. This time with carry. 
 *
 * A standard 8 bit subtract, this time with a twist. The value passed in is simply an address
 * in a register and we have to fetch the actual value from memory and then perform the 
 * subtract. 
 * The result is A -> A - *address_register - carry.
 * This function will be used to implement the following opcodes:
 * 		0x96
 * 
 * @param accumulator: Pointer to the accumulator register: A.
 * @param address_register: Pointer to 16 bit register. Value in this register will be stored used in calculation 
 * @param flags: Pointer to the status flags register. 
 */
void subtract_indirect_with_carry(Register8 *accumulator, Register16 *address_register, Register8 *flags) {
	unsigned char carry = (*flags & 0x10) >> 4;
	unsigned char value_at_address = read_byte(*address_register);
	unsigned char result = *accumulator - value_at_address - carry;

	// Set flags and accumulator and be done
	*flags = set_flags_sub(result, *accumulator);
	*accumulator = result;
}

// Bitwise Operators //

/**
 * /brief A bitwise AND where the operand is stored within a register
 * 
 * Performs a bitwise AND where the operand is stored within a register
 * This function implements the following opcodes:
 *		A7, A0, A1, A2, A3, A4, A5
 *
 * @param: accumulaor: Pointer to the accumulator. This is one operand. 
 * 		The result is also stored within this register. 
 * @param: other_register: Pointer to the register containing the other operand
 * @param: flags: Pointer to the flags register
 */
void bitwise_and_register(Register8 *accumulator, Register8 *other_register, Register8 *flags) {
	*accumulator = *accumulator & *other_register;
	*flags = 0x20 | ((*accumulator == 0) << 7);	// We always set the half carry. Only throw the Z flag if result is 0
}

/**
 * /brief Performs a bitwise and where the other operand is an immediate operand
 * 
 * Performs a bitwise and where the other operand is an immediate operand
 * This function implements the following opcodes: 
 *		E6
 * 
 * @param: accumulator: Pointer to the accumulator. This is both the first operand 
 * 		as well as where the result of the operation will be stored
 * @param: value: The second operand in our bitwise AND
 * @param: flags: Pointer to the flags register
 */
void bitwise_and_immediate(Register8 *accumulator, unsigned char value, Register8 *flags) {
	*accumulator = *accumulator & value;
	*flags = 0x20 | ((*accumulator == 0) << 7);	// We always set the half carry. Only throw the Z flag if result is 0
}

/**
 * /brief A bitwise AND with indirect addressing
 * 
 * Performs a bitwise AND with indirect addressing. Per the specs of the z80,
 * the other operand is found within the HL register.
 * This function implements the following opcodes: 
 * 		A6
 *
 * @param: accumulator: Pointer to the accumulator. This is both the first operand 
 * 		as well as where the result of the operation will be stored
 * @param: address_register: Pointer to the HL register with the address containing the value
 * 		we wish to use as an operand
 * @param: flags: Pointer to the flags register
 */
void bitwise_and_indirect(Register8 *accumulator, Register16 *address_register, Register8 *flags) {
	unsigned char value_at_address = read_byte(*address_register);
	*accumulator = *accumulator & value_at_address;
	*flags = 0x20 | ((*accumulator == 0) << 7);	// We always set the half carry. Only throw the Z flag if result is 0
}

/**
 * /brief Bitwise OR with second operand within a register
 * 
 * Performs a bitwise OR with the second operand found in a register specified by the supplied opcode
 * This function implements the following opcodes:
 *		B7, B0, B1, B2, B3, B4, B5
 *
 * @param: accumulaor: Pointer to the accumulator. This is one operand. 
 * 		The result is also stored within this register. 
 * @param: other_register: Pointer to the register containing the other operand
 * @param: flags: Pointer to the flags register
 */
void bitwise_or_register(Register8 *accumulator, Register8 *other_register, Register8 *flags) {
	*accumulator = *accumulator | *other_register;
	*flags = (*accumulator == 0) << 7;		// Set 0 flag if result is 0
}

/**
 * /brief Bitwise OR with second operand is an immediate value
 * 
 * Performs a bitwise OR with the second operand is an immediate value
 * This function implements the following opcodes:
 *		F6
 *
 * @param: accumulaor: Pointer to the accumulator. This is one operand. 
 * 		The result is also stored within this register. 
 * @param: value: The second operand in our bitwise OR
 * @param: flags: Pointer to the flags register
 */
void bitwise_or_immediate(Register8 *accumulator, unsigned char value, Register8 *flags) {
	*accumulator = *accumulator | value;
	*flags = (*accumulator == 0) << 7;		// Set 0 flag if result is 0
}

/**
 * /brief A bitwise OR with indirect addressing
 * 
 * Performs a bitwise OR with indirect addressing. Per the specs of the z80,
 * the other operand is found within the HL register.
 * This function implements the following opcodes: 
 * 		F6
 *
 * @param: accumulator: Pointer to the accumulator. This is both the first operand 
 * 		as well as where the result of the operation will be stored
 * @param: address_register: Pointer to the HL register with the address containing the value
 * 		we wish to use as an operand
 * @param: flags: Pointer to the flags register
 */
void bitwise_or_indirect(Register8 *accumulator, Register16 *address_register, Register8 *flags) {
	unsigned char value_at_address = read_byte(*address_register);
	*accumulator = *accumulator | value_at_address;
	*flags = (*accumulator == 0) << 7;	// We always set the half carry. Only throw the Z flag if result is 0
}

/**
 * /brief Bitwise XOR with second operand within a register
 * 
 * Performs a bitwise XOR with the second operand found in a register specified by the supplied opcode
 * This function implements the following opcodes:
 *		AF, A8, A9, AA, AB, AC, AD
 *
 * @param: accumulaor: Pointer to the accumulator. This is one operand. 
 * 		The result is also stored within this register. 
 * @param: other_register: Pointer to the register containing the other operand
 * @param: flags: Pointer to the flags register
 */
void bitwise_xor_register(Register8 *accumulator, Register8 *other_register, Register8 *flags)  {
	*accumulator = *accumulator ^ *other_register;
	*flags = (*accumulator == 0) << 7;
}

/**
 * /brief Bitwise XOR with second operand is an immediate value
 * 
 * Performs a bitwise XOR with the second operand is an immediate value
 * This function implements the following opcodes:
 *		EE
 *
 * @param: accumulaor: Pointer to the accumulator. This is one operand. 
 * 		The result is also stored within this register. 
 * @param: value: The second operand in our bitwise XR
 * @param: flags: Pointer to the flags register
 */
void bitwise_xor_immediate(Register8 *accumulator, unsigned char value, Register8 *flags) {
	*accumulator = *accumulator ^ value;
	*flags = (*accumulator == 0) << 7;
}

/**
 * /brief A bitwise XOR with indirect addressing
 * 
 * Performs a bitwise XOR with indirect addressing. Per the specs of the z80,
 * the other operand is found within the HL register.
 * This function implements the following opcodes: 
 * 		AE
 *
 * @param: accumulator: Pointer to the accumulator. This is both the first operand 
 * 		as well as where the result of the operation will be stored
 * @param: address_register: Pointer to the HL register with the address containing the value
 * 		we wish to use as an operand
 * @param: flags: Pointer to the flags register
 */
void bitwise_xor_indirect(Register8 *accumulator, Register16 *address_register, Register8 *flags) {
	unsigned char value_at_address = read_byte(*address_register);
	*accumulator = *accumulator ^ value_at_address;
	*flags = (*accumulator == 0) << 7;	// We always set the half carry. Only throw the Z flag if result is 0
}

// COMPARISONS 

/**
 * /brief A comparison between the accumulator and a value stored within a register
 *
 * Performs a comparison between the value in the accumulator and a value 
 * stored within another register. Essentially it does A - R but the result is not 
 * stored within the accumulator.
 * This function implements the following opcodes: 
 *		BF, B8, B9, BA, BC, BD
 *
 * @param accumulator: Pointer to the accumulator register: A.
 * @param value: 8-bit value we wish to compare with the accumulator. Unsigned
 * @param flags: Pointer to the status flags register. 
 */
void compare_register(Register8 *accumulator, Register8 *other_register, Register8 *flags) { 
	unsigned char result = *accumulator - *other_register;	// Like above, we are not doing immediate storage

	// Set flags but disregard the result
	*flags = set_flags_sub(result, *accumulator);
}

/**
 * /brief Compares accumulator with immediate 8 bit value
 *
 * Compares the value stored in the accumulator with a directly supplied
 * immediate value.
 * This function implements the following opcodes: 
 *		FE
 *
 * Note: the second operand will be expected to be found in the next byte in the 
 *		game's source code.
 * 
 * @param accumulator: Pointer to the accumulator register: A.
 * @param value: 8-bit value we wish to compare with the accumulator. Unsigned
 * @param flags: Pointer to the status flags register. 
 */
void compare_immediate(Register8 *accumulator, unsigned char value, Register8 *flags) {
	unsigned char result = *accumulator - value;	// Like above, we are not doing immediate storage

	// Set flags but disregard result. 
	*flags = set_flags_sub(result, *accumulator);
}

/** 
 * /brief: Compares the value stored in the accumulator with a value stored in memory
 * 
 * A comparison of the value in the accumulator with a value stored in memory. This value 
 * is accessed using indirect addressing using the value stored within the HL register. 
 * This function implements the following opcodes: 
 * 		BE
 * @param accumulator: Pointer to the accumulator register: A.
 * @param address_register: Pointer to the "16 bit register" HL. This will tell us where 
 * 		to find our operand in memory.
 * @param flags: Pointer to the status flags register.
 */
void compare_indirect(Register8 *accumulator, Register16 *address_register, Register8 *flags) {
	unsigned char value_at_address = read_byte(*address_register);
	unsigned char result = *accumulator - value_at_address;

	// Set flags, disregard accumulator
	*flags = set_flags_sub(result, *accumulator);
}

/**
 * /brief Increments the value found within a given register.
 *
 * Increments the value stored in the provided register by one.
 * This function implements the following opcodes: 
 * 		3C, 04, 0C, 14, 1C, 24, 2C
 *
 * @param: target_register: A pointer to the register being incremented
 * @param: flags: Pointer to the flags register
 */
void increment_register(Register8 *target_register, Register8 *flags) {
	add_immediate(target_register, 1, flags);
}

/**
 * /brief Increments a value stored in memory through indirect addressing.
 *
 * Increments a value stored in memory and then saves the updated value back to 
 * memory.
 * This function implements the following opcodes: 
 *		34
 *
 * @param address_register: The HL register where the memory address we will access 
 * 		is stored as a value.
 * @param flags: Pointer to the flags register
 */
void increment_register_indirect(Register16 *address_register, Register8 *flags) {
	unsigned char value_at_address = read_byte(*address_register);
	
	// Creating a pseudo register we can call the "accumulator" for this operation
	Register8 pseudo_accumulator = value_at_address;
	add_immediate(&pseudo_accumulator, 1, flags);
	write_byte(*address_register, pseudo_accumulator);
}

/**
 * /brief Decrements the value found within a given register.
 *
 * Decrements the value stored in the provided register by one.
 * This function implements the following opcodes: 
 * 		3C, 04, 0C, 14, 1C, 24, 2C
 *
 * @param: target_register: A pointer to the register being decremented
 * @param: flags: Pointer to the flags register
 */
void decrement_register(Register8 *target_register, Register8 *flags) {
	subtract_immediate(target_register, 1, flags);
}

/**
 * /brief Decrements a value stored in memory through indirect addressing.
 *
 * Decrements a value stored in memory and then saves the updated value back to 
 * memory.
 * This function implements the following opcodes: 
 *		35
 * @param address_register: The HL register where the memory address we will access 
 * 		is stored as a value.
 * @param flags: Pointer to the flags register
 */
void decrement_register_indirect(Register16 *address_register, Register8 *flags) {
	unsigned char value_at_address = read_byte(*address_register);
	
	// Creating a pseudo register we can call the "accumulator" for this operation
	Register8 pseudo_accumulator = value_at_address;
	subtract_immediate(&pseudo_accumulator, 1, flags);
	write_byte(*address_register, pseudo_accumulator);
}

/**
 * /brief Sets the flags after an 8bit addition operation
 * 
 * Per the specs of the Zilog z80 processor: Z will be set if result is 0, N will be reset
 * H will be set if there's a carry out from bit 3 in the add and C will be set if there's
 * a carry from bit 7.
 *
 * @param result: The result of the addition operation
 * @param old_accumulator: The value in the accumulator before the add
 *
 * @return: The new value to be stored in the flags register. 
 */
Register8 set_flags_add(Register8 result, Register8 old_accumulator) {
	// Constructing the new flag values
	Register8 new_flags = 0;
	new_flags |= ((result == 0) << ZERO_FLAG_POS);				// Set Zero flag
	new_flags |= ((result < old_accumulator)) << CARRY_FLAG_POS;	// Set Carry Flag

	// The half carry flag is a bit more sophisticated. We must look at the 
	// lower nibble of the both the result and the accumulator. If the result's 
	// lower nibble is less than that of the accumulator then we know to set our 
	// half carry flag. One should note the similarity to how we do the full 8-bit
	// carry flag above.
	Register8 lower_nibble_result = result & 0x0F;
	Register8 lower_nibble_accumulator = old_accumulator & 0x0F;

	// Useful output for debugging
	#ifdef VERBOSE
		printf("Result: %d, Old Accumulator: %d\n", result, old_accumulator);
		printf("Lower Nibble Result: %d, Lower Nibble Accumulator: %d\n", lower_nibble_result, lower_nibble_accumulator);
	#endif
	
	new_flags |= ((lower_nibble_result < lower_nibble_accumulator) << HALF_CARRY_FLAG_POS);
	return new_flags;
}

/**
 * /brief Sets the flags after an 8bit subtraction operation
 * 
 * Per the specs of the Zilog z80 processor: Z will be set if the result is zero, 
 * N will be set (as the operation was a subtraction), H will be set if there was no
 * borrow from bit 4 while C will be set if there was no borrow in the overall operation
 *
 * @param reuslt: The result of the subtraction operation
 * @param old_accumulator: The value in the accumulator before the subtraction
 *
 * @result: The new value to be stored in the flags register
 */
Register8 set_flags_sub(Register8 result, Register8 old_accumulator) {
	// Constructing the new flag values
	Register8 new_flags = 0;

	new_flags |= (result == 0) << ZERO_FLAG_POS;				// Set Zero Flag
	new_flags |= 0x1 << SUB_FLAG_POS;							// Set Subtraction Flag
	new_flags |= (result > old_accumulator) << CARRY_FLAG_POS;	// Set Carry Flag

	// The half carry flag is a bit more sophisticated. We must look at the 
	// lower nibble of the both the result and the accumulator. If the result's 
	// lower nibble is greater than that of the accumulator then we know to set our 
	// half carry flag. One should note the similarity to how we do the full 8-bit
	// carry flag above.
	Register8 lower_nibble_result = result & 0x0F;
	Register8 lower_nibble_accumulator = old_accumulator & 0x0F;

	// Useful output for debugging
	#ifdef VERBOSE
		printf("Result: %d, Old Accumulator: %d\n", result, old_accumulator);
		printf("Lower Nibble Result: %d, Lower Nibble Accumulator: %d\n", lower_nibble_result, lower_nibble_accumulator);
	#endif

	new_flags |= ((lower_nibble_result > lower_nibble_accumulator) << HALF_CARRY_FLAG_POS);
	return new_flags;
}


/*** 16-BIT ARITHMETIC ***/

/**
 * /brief Performs a 16 bit add operation where the HL register is treated
 * 		akin to the accumulator
 *
 * This function performs a 16 bit add operation where the HL register is the destination
 * of the result. The second operand is the value stored in one of the other 
 * 16 bit "registers".
 * This function implements the following opcodes:
 *		09, 19, 29, 39
 *
 * @param indirirect_address_register: Pointer to the HL register. Nothing actually enforces this, so it must 
 * 		be ensured by the programmer in the calling environment
 * @param other_regisrer: Pointer to the 16 bit register containing the second operand
 * @param flags: Pointer to the flags register
 */
void indirect_register_add(Register16 *indirect_address_register, Register16 *other_register, Register8 *flags) {
	unsigned short result = *indirect_address_register + *other_register;

	// Setting flags
	unsigned char new_flags = 0x00;

	// Set the carry flag.
	new_flags = new_flags | (result < *indirect_address_register) << 4;

	// Setting Half Carry Flag (set if carry out of bit 11)
	unsigned short lower_twelve_result = result & 0x0FFF;
	unsigned short lower_twelve_accumulator = *indirect_address_register & 0x0FFF;
	new_flags = new_flags | (lower_twelve_result < lower_twelve_accumulator) << 5;

	// The operation preserves the zero flag from the last op
	new_flags = new_flags | (*flags & 0x80);

	// Set register and flags
	*flags = new_flags;
	*indirect_address_register = result;
}

/**
 * /brief Adds an 8 bit immediate value to the stack pointer
 *
 * Performs an 8 bit add to value contained in the stack pointer.
 * This function implements the following opcodes:
 *		E8
 *
 * @param indirirect_address_register: Pointer to the stack pointer. Nothing actually enforces this, so it must 
 * 		be ensured by the programmer in the calling environment
 * @param other_regisrer: 8 bit value we wish to add to the stack pointer. 
 * @param flags: Pointer to the flags register
 */
void stack_pointer_add(Register16 *stack_pointer, unsigned char value, Register8 *flags) {
	unsigned short result = *stack_pointer + value;

	// Setting flags
	unsigned char new_flags = 0x00;

	// Set the carry flag.
	new_flags = new_flags | (result < *stack_pointer) << 4;

	// Setting Half Carry Flag (set if carry out of bit 11)
	unsigned short lower_twelve_result = result & 0x0FFF;
	unsigned short lower_twelve_accumulator = *stack_pointer & 0x0FFF;
	new_flags = new_flags | (lower_twelve_result < lower_twelve_accumulator) << 5;

	// Set register and flags
	*flags = new_flags;
	*stack_pointer = result;
}

/**
 * /brief Increments a value in a 16 bit register
 *
 * Increments a value found within a 16 bit register.
 * This function does not make any changes to the flags.
 * Thus function implements the following opcodes: 
 * 		03, 13, 23, 33
 *
 * @param target_register: Pointer to 16 bit register whose 
 * 		value we are incrementing
 */
void increment_register_16(Register16 *target_register) {
	*target_register += 1;
}

/**
 * /brief Decrements a value in a 16 bit register
 *
 * Decrements a value found within a 16 bit register.
 * This function does not make any changes to the flags.
 * This function implements the following opcodes: 
 * 		0B, 1B, 2B, 3B
 *
 * @param target_register: Pointer to 16 bit register whose 
 * 		value we are decrementing
 */
void decrement_register_16(Register16 *target_register) {
	*target_register-= 1;
}

/*** ROTATES, SHIFTS OTHER BIT FUN ***/

/**
 * /brief Rotates the supplied register left by one. 7 bit to carry register
 * 
 * Rotates the supplied register left by one. 7 bit to carry 
 * register where it's archived.
 * This function implements the following opcodes:
 *		07, CB 07, CB 00, CB 01, CB 02, CB 03, CB 04, CB 05
 *
 * @param target_register: The register whose value we will be rotating
 * @param flags: Pointer to the flags register
 */
void rotate_register_left_carry_archive(Register8 *target_register, Register8 *flags) {
	// Do the rotate
	*target_register = ((*target_register << 1) | (*target_register >> 7));

	// Set the flags register
	*flags = (*target_register == 0) << 7;			// Setting 0 bit
	*flags |= ((*target_register & 0x01) << 4);		// 7 bit to carry
}

/**
 * /brief Left Rotates value in memory address contained in the "HL" address register. 
 * 7 bit to carry register
 * 
 * Rotates the value stored in the supplied memory address
 * left by one. 7 bit to carry register where it's archived.
 * This function implements the following opcodes:
 *		CB 06
 * @param address_register: Address register. Contains address of value we wish to manipulate
 * @param flags: Pointer to the flags register
 */
void rotate_indirect_left_carry_archive(Register16 *address_register, Register8 *flags) {
	unsigned char value_at_address = read_byte(*address_register);

	// Do the rotate
	value_at_address = ((value_at_address << 1) | (value_at_address >> 7));

	// Set the flags register
	*flags = (value_at_address == 0) << 7;			// Setting 0 bit
	*flags |= ((value_at_address & 0x01) << 4);	// 7 bit to Carry

	// Write it back to memory.
	write_byte(*address_register, value_at_address);
}	

/**
 * /brief Rotates the supplied register left through the carry bit
 * 
 * Rotates the supplied register left by one. This rotate
 * goes through the carry bit. 
 * This function implements the following opcodes:
 *		17, CB 17, CB 10, CB 11, CB 12, CB 13, CB 14, CB 15
 *
 * @param target_register: The register whose value we will be rotating
 * @param flags: Pointer to the flags register
 */
void rotate_register_left_through_carry(Register8 *target_register, Register8 *flags) {
	unsigned char old_carry = (*flags & 0x10) >> 4;
	unsigned char new_carry = (*target_register & 0x80) >> 3;	// Move bit 7 into carry flag pos

	*target_register = (*target_register << 1) | old_carry;	// Shift and include archived carry bit.
	*flags = new_carry | ((*target_register == 0) << 7);
}

/**
 * /brief Left Rotates value in memory address contained in the "HL" address register.
 * This rotate goes through the carry bit. 
 * 
 * Rotates the value stored in the supplied memory address
 * right by one. 7 bit to carry register where it's archived.
 * This function implements the following opcodes:
 *		CB 16
 * @param address_register: Address register. Contains address of value we wish to manipulate
 * @param flags: Pointer to the flags register
 */
void rotate_indirect_left_through_carry(Register16 *address_register, Register8 *flags) {
	unsigned char value_at_address = read_byte(*address_register);

	unsigned char old_carry = (*flags & 0x10) >> 4;
	unsigned char new_carry = (value_at_address & 0x80) >> 3;	// Move bit 7 into carry flag pos

	value_at_address = (value_at_address << 1) | old_carry;	// Shift and include archived carry bit.
	*flags = new_carry | ((value_at_address == 0) << 7);	

	write_byte(*address_register, value_at_address);
}
/**
 * /brief Rotates the supplied register right by one. 0 bit to carry register
 * 
 * Rotates the supplied register right by one. 0 bit to carry 
 * register where it's archived.
 * This function implements the following opcodes:
 *		0F, CB 0F, CB 08, CB 09, CB 0A, CB 0B, CB 0C, CB 0D
 *
 * @params target_register: The register whose value we will be rotating
 * @params flags: Pointer to the flags register
 */
void rotate_register_right_carry_archive(Register8 *target_register, Register8 *flags) {
	// Do the rotate
	*target_register = ((*target_register >> 1) | (*target_register << 7));

	// Set the flags register
	*flags = (*target_register == 0) << 7;			// Setting 0 bit
	*flags |= (*target_register & 0x80) >> 3;		// Move old LSB into carry
}

/**
 * /brief Right Rotates value in memory address contained in the "HL" address register. 
 * 0 bit to carry register
 * 
 * Rotates the value stored in the supplied memory address
 * right by one. 0 bit to carry register where it's archived.
 * This function implements the following opcodes:
 *		CB 0E
 * @param address_register: Address register. Contains address of value we wish to manipulate
 * @param flags: Pointer to the flags register
 */
void rotate_indirect_right_carry_archive(Register16 *address_register, Register8 *flags) {
	unsigned char value_at_address = read_byte(*address_register);

	// Do the rotate
	value_at_address = ((value_at_address >> 1) | (value_at_address << 7));

	// Set the flags register
	*flags = (value_at_address == 0) << 7;			// Setting 0 bit
	*flags |= ((value_at_address & 0x80) >> 3);		// Moving old LSB to carry bit

	// Write it back to memory.
	write_byte(*address_register, value_at_address);
}	

/**
 * /brief Rotates the supplied register right through the carry bit
 * 
 * Rotates the supplied register right by one. This rotate
 * goes through the carry bit. 
 * This function implements the following opcodes:
 *		1F, CB 1F, CB 18, CB 19, CB 1A, CB 1B, CB 1C, CB 1D
 *
 * @param target_register: The register whose value we will be rotating
 * @param flags: Pointer to the flags register
 */
void rotate_register_right_through_carry(Register8 *target_register, Register8 *flags) {
	unsigned char old_carry = (*flags & 0x10) << 3;				// Move old carry to 7 position
	unsigned char new_carry = (*target_register & 0x01) << 4;	// Move bit 0 into carry flag pos

	*target_register = (*target_register >> 1) | old_carry;	// Shift and include archived carry bit.
	*flags = new_carry | ((*target_register == 0) << 7);
}

/**
 * /brief Right Rotates value in memory address contained in the "HL" address register.
 * This rotate goes through the carry bit. 
 * 
 * Rotates the value stored in the supplied memory address
 * right by one. 7 bit to carry register where it's archived.
 * This function implements the following opcodes:
 *		CB 1E
 * @param address_register: Address register. Contains address of value we wish to manipulate
 * @param flags: Pointer to the flags register
 */
void rotate_indirect_right_through_carry(Register16 *address_register, Register8 *flags) {
	unsigned char value_at_address = read_byte(*address_register);

	unsigned char old_carry = (*flags & 0x10) << 3;				// Move old carry to 7 position
	unsigned char new_carry = (value_at_address & 0x01) << 4;	// Move bit 0 into carry flag pos

	value_at_address = (value_at_address >> 1) | old_carry;	// Shift and include archived carry bit.
	*flags = new_carry | ((value_at_address == 0) << 7);

	write_byte(*address_register, value_at_address);
}

/**
 * /brief Shifts provided register left by one. MSB to carry
 *
 * Shifts a value stored in a register one position to the left.
 * The MSB of the previous word is stored in the carry bit of the 
 * flags register.
 * This function implements the following opcodes:
 * 		CB 27, CB 20, CB 21, CB 22, CB 23, CB 24, CB 25 
 *
 * @param target_register: The register with the operand of the operation
 * @param flags: Pointer to the flags register
 */
void shift_register_left(Register8 *target_register, Register8 *flags) {
	// Preserving the MSB in the carry flag
	*flags = (*target_register & 0x80) >> 3;	// Move MSB to the carry position
	*target_register = *target_register << 1;	// Perform the shift
	*flags |= (*target_register == 0) << 7;		// Check if result is zero
}

/**
 * /brief Shifts value at provided memory address left by one. MSB to carry
 *
 * Shifts a value stored in a memory address one position to the left.
 * The MSB of the previous word is stored in the carry bit of the 
 * flags register. The location of the value is pointed to by the 
 * "HL" register.
 * This function implements the following opcodes:
 * 		CB 26
 * 
 * @param address_register: Contains the address of our operand
 * @param flags: Pointer to the flags register
 */
void shift_indirect_left(Register16 *address_register, Register8 *flags) {
	unsigned char value_at_address = read_byte(*address_register);

	// Preserving the MSB in the carry flag
	*flags = (value_at_address & 0x80) >> 3;	// Move MSB to the carry position
	value_at_address = value_at_address << 1;	// Perform the shift
	*flags |= (value_at_address == 0) << 7;		// Check if result is zero

	write_byte(*address_register, value_at_address);
}

/**
 * /brief Arithmetically Shifts provided register right by one. LSB to carry
 *
 * Shifts a value stored in a register one position to the right.
 * The LSB of the previous word is stored in the carry bit of the 
 * flags register. Remember, the MSB is propagated
 * This function implements the following opcodes:
 * 		CB 2F, CB 28. CB 29, CB 2A, CB 2B, CB 2C, CB 2D 
 *
 * @param address_register: Contains the address of our operand
 * @param flags: Pointer to the flags register
 */
void arithmetic_shift_register_right(Register8 *target_register, Register8 *flags) {
	// Since we want to do an arithmetic shift we have to beat around the 
	// bush..
	unsigned char most_sig_bit = *target_register & 0x80;

	// Now we do a standard shift
	*flags = (*target_register & 0x01) << 4;	// Set carry flag with LSB
	*target_register = (*target_register >> 1) | most_sig_bit;
	*flags |= (*target_register == 0) << 7;		// Set zero flag
}

/**
 * /brief Arithmetically Shifts a value received through indirect addressing
 *  right by one. LSB to carry
 *
 * Arithmetically Shifts a value stored in a register one position to the right.
 * The LSB of the previous word is stored in the carry bit of the 
 * flags register. Remember, the MSB propagated
 * This function implements the following opcodes:
 * 		CB 2E 
 *
 * @param target_register: The register with the operand of the operation
 * @param flags: Pointer to the flags register
 */
void arithmetic_shift_indirect_right(Register16 *address_register, Register8 *flags) {
	unsigned char value_at_address = read_byte(*address_register);

	// Since we want to do an arithmetic shift we have to beat around the 
	// bush..
	unsigned char most_sig_bit = value_at_address & 0x80;

	// Now we do a standard shift
	*flags = (value_at_address & 0x01) << 4;	// Set carry flag with LSB
	value_at_address = (value_at_address >> 1) | most_sig_bit;
	*flags |= (value_at_address == 0) << 7;		// Set zero flag
	
	write_byte(*address_register, value_at_address);
}

/**
 * /brief Logically Shifts provided register right by one. 
 * LSB to carry
 *
 * Shifts a value stored in a register one position to the right.
 * The LSB of the previous word is stored in the carry bit of the 
 * flags register.
 * This function implements the following opcodes:
 * 		CB 3F, CB CB 38, CB 39, CB 3A, CB 3B, CB 3C, CB 3D
 *
 * @param target_register: The register with the operand of the operation
 * @param flags: Pointer to the flags register
 */
void logical_shift_register_right(Register8 *target_register, Register8 *flags) {
	// Preserving the LSB in the carry flag
	*flags = (*target_register & 0x01) << 4;	// Move >SB to the carry position
	*target_register = *target_register >> 1;	// Perform the shift
	*flags |= (*target_register == 0) << 7;		// Check if result is zero
}

/**
 * /brief Logically Shifts value at provided memory address right by one. 
 * LSB to carry
 *
 * Shifts a value stored in a memory address one position to the right.
 * The LSB of the previous word is stored in the carry bit of the 
 * flags register. The location of the value is pointed to by the 
 * "HL" register.
 * This function implements the following opcodes:
 * 		CB 3E
 * 
 * @param address_register: Contains the address of our operand
 * @param flags: Pointer to the flags register
 */
void logical_shift_indirect_right(Register16 *address_register, Register8 *flags) {
	unsigned char value_at_address = read_byte(*address_register);

	// Preserving the MSB in the carry flag
	*flags = (value_at_address & 0x01) << 4;	// Move LSB to the carry position
	value_at_address = value_at_address >> 1;	// Perform the shift
	*flags |= (value_at_address == 0) << 7;		// Check if result is zero

	write_byte(*address_register, value_at_address);
}

/*** BIT OPCODES ***/

/*
 * /brief Tests bit "bit" in the specified register
 * 
 * This implements functionality where we can check the 
 * value of any bit within a word. Note, if bit is 
 * between 0 and 7 (inclusive), this function will cause the 
 * programme to abort. 
 * This function implements the following op codes
 * CB 47, CB 40, CB 41, CB 42, CB 43, CB 44, CB 45
 * 
 * @param target_value: The register containing the value we are testing
 * @param bit: The bit we wish to test. Must be between 0-7 (inclusive)
 * @param flags: Pointer to the flags register
 */
void test_bit_register(Register8 *target_register, unsigned char bit, Register8 *flags) {
	// As mentioned previously, we abort if we've erroneous operands.
	if (bit < 0 || 7 < bit) {
		fprintf(stderr, "ILLEGAL OPERATION: Called BIT %d, %d\n", *target_register, bit);
		abort();
	}
	// implicit else. 
	unsigned char target_value = *target_register >> bit; // Assummes bit 0 is LSB
	*flags = 0x20 || (target_value == 0);	// Half carry always set, Z if bit is 0
}

/*
 * /brief Tests bit "bit" in a specified memory location.
 * 
 * This implements functionality where we can check the 
 * value of any bit within a word. Except, this time the word 
 * is accessed through indirect addressing! Note, if bit is 
 * between 0 and 7 (inclusive), this function will cause the 
 * programme to abort. 
 * This function implements the following op codes
 * CB 3E
 * 
 * @param address_register: A register containing an indirect reference to the
 * 						value we wish to modify.
 * @param bit: The bit we wish to test. Must be between 0-7 (inclusive)
 * @param flags: Pointer to the flags register
 */
void test_bit_indirect(Register16 *address_register, unsigned char bit, Register8 *flags) {
	
	// As mentioned previously, we abort if we've erroneous operands.
	if (bit < 0 || 7 < bit) {
		fprintf(stderr, "ILLEGAL OPERATION: Called BIT %d, %d\n", *address_register, bit);
		abort();
	}
	// Implicit else.
	unsigned char target_value = read_byte(*address_register);
	 
	target_value = target_value >> bit; // Assummes bit 0 is LSB
	*flags = 0x20 || (target_value == 0);	// Half carry always set, Z if bit is 0

}

/**
 * /brief Sets the bit-th bit in the target register to one.
 * 
 * Sets the bit at position "bit" in the target register to one.
 * This function implements the following op codes:
 *		CB C7, CB C0, CB C1, CB C2, CB C3, CB C4, CB C5
 *
 * @param target_register: Pointer to the register with the value we wish to 
 * 		manipulate.
 * @param bit: The bit we wish to set. If this value is not between 0-7 (inclusive),
 * 		the process will abort.
 */
void set_bit_register(Register8 *target_register, unsigned char bit) {
	// Ensure we have a correct "bit" value
	if (bit < 0 || 7 < bit) {
		fprintf(stderr, "ILLEGAL OPERATION: Called SET on the %d 'th bit.", bit);
		abort();
	}

	// Implicit else
	*target_register |= (0x01 << bit);		// This will set the n'th bit
}

/**
 * /brief Sets the bit-th bit at the memory address in the address_register to one.
 * 
 * Leverages indirect addressing to set a given bit at a target memory address to one. 
 * This function implements the following opcodes:
 * 		CB C6
 * 
 * @param address_register: HL register. Contains the bit we wish to set
 * @param bit: The bit we wish to set. If this value is not between 0-7 (inclusive),
 * 		the process will abort.
 */
void set_bit_indirect(Register16 *address_register, unsigned char bit) {
	// Ensure we have a correct "bit" value
	if (bit < 0 || 7 < bit) {
		fprintf(stderr, "ILLEGAL OPERATION: Called SET on the %d 'th bit.", bit);
		abort();
	}
	// Implicit else
	unsigned char value_at_address = read_byte(*address_register);
	value_at_address |= (0x01 << bit);		// This will set the n'th bit
	write_byte(*address_register, value_at_address);
}

/**
 * /brief Resets the bit-th bit in the value at the target register
 *
 * Resets the bit-th bit in the value at the supplied register.
 * This function implements the following opcodes:
 * 		CB 87, CB 80, CB 81, CB 82, CB 83, CB 84, CB 85 
 *
 * @param target_register: Pointer to the register whose value we wish to reset
 * @param bit: The bit we wish to manipulate. If this value is not between 0-7 (inclusive),
 * 		the process will abort.
 */
void reset_bit_register(Register8 *target_register, unsigned char bit) {
	// Ensure we have a correct "bit" value
	if (bit < 0 || 7 < bit) {
		fprintf(stderr, "ILLEGAL OPERATION: Called RES on the %d 'th bit.", bit);
		abort();
	}
	// Implicit Else
	unsigned char mask = ~(0x01 << bit);		// Explicitly defining our mask. It's cleaner
	*target_register &= (mask);
}

/**
 * /brief Resets the bit-th bit at the memory address in the address_register to zero.
 * 
 * Leverages indirect addressing to set a given bit at a target memory address to zero.
 * This function implements the following opcodes:
 * 		CB 86 
 *
 * @param address_register: HL register. Contains the bit we wish to reset
 * @param bit: The bit we wish to reset. If this value is not between 0-7 (inclusive),
 * 		the process will abort.
 */
void reset_bit_indirect(Register16 *address_register, unsigned char bit) {
	// Ensure we have a correct "bit" value
	if (bit < 0 || 7 < bit) {
		fprintf(stderr, "ILLEGAL OPERATION: Called RES on the %d 'th bit.", bit);
		abort();
	}
	// Implicit else
	unsigned char value_at_address = read_byte(*address_register);
	unsigned char mask = ~(0x01 << bit);
	value_at_address &= mask;
	write_byte(*address_register, value_at_address);	
}

/*** JUMPS ***/

/**
 * /brief Sets the programme counter to the addressed referenced in the operand.
 *
 * Sets the programme counter to the address in the supplied operand. 
 * Important to note is that the second operand is big-endian while the memory interface 
 * of this emulator is little endian. Thus a conversion where the LSB and MSB of the 
 * supplied operand will have to be swapped.
 * 
 * This function implements the following opcodes:
 * 		C3
 *
 * @param programme_counter: Pointer to the programme counter
 * @param address_little_endian: The new value of the programme counter. Big endian.
 */
void jump_unconditional(Register16 *programme_counter, unsigned short address_big_endian) {
	unsigned short address = to_little_endian(address_big_endian);
	*programme_counter = address;		// Update our programme counter
}

/**
 * /brief Jumps to the specified address if the zero flag is not set. 
 * 
 * Sets the programme counter to the supplied address conditional upon the 
 * zero flag being reset. As with all jumps it is important to note that the second 
 * address is big-endian while the programme's own memory interface is little endian.
 * Thus the MSB and LSB must be swapped. 
 * 
 * This function implements the following opcodes:
 *		C2
 *
 * @param programe_counter: Pointer to the programme counter
 * @param address_little_endian: The new value of the programme counter. Big endian.
 * @param flags: Pointer to the flags register
 */
void jump_zero_reset(Register16 *programme_counter, unsigned short address_big_endian, Register8 *flags) {
	unsigned short address = to_little_endian(address_big_endian);
	
	// Flip bits and then mask for Zero. 
	if (!zero_flag_set(flags)) {
		*programme_counter = address;		// Update our programme counter
	}
}

/**
 * /brief Jumps to the specified address if the zero flag is set. 
 * 
 * Sets the programme counter to the supplied address conditional upon the 
 * zero flag being set. As with all jumps it is important to note that the second 
 * address is big-endian while the programme's own memory interface is little endian.
 * Thus the MSB and LSB must be swapped. 
 * 
 * This function implements the following opcodes:
 *		CA
 *
 * @param programe_counter: Pointer to the programme counter
 * @param address_little_endian: The new value of the programme counter. Big endian.
 * @param flags: Pointer to the flags register
 */
void jump_zero_set(Register16 *programme_counter, unsigned short address_big_endian, Register8 *flags) {
	unsigned short address = to_little_endian(address_big_endian);
	
	// Mask out everything but the Zero Flag. 
	// if the number is non zero, jump
	if (zero_flag_set(flags)) {
		*programme_counter = address;		// Update our programme counter
	}
}

/**
 * /brief Jumps to the specified address if the carry flag is reset. 
 * 
 * Sets the programme counter to the supplied address conditional upon the 
 * carry flag being set. As with all jumps it is important to note that the second 
 * address is big-endian while the programme's own memory interface is little endian.
 * Thus the MSB and LSB must be swapped. 
 * 
 * This function implements the following opcodes:
 *		D2
 *
 * @param programe_counter: Pointer to the programme counter
 * @param address_little_endian: The new value of the programme counter. Big endian.
 * @param flags: Pointer to the flags register
 */
void jump_carry_reset(Register16 *programme_counter, unsigned short address_big_endian, Register8 *flags) {
	unsigned short address = to_little_endian(address_big_endian);
	
	// Flip bits and then mask for carry
	if (!carry_flag_set(flags)) {
		*programme_counter = address;		// Update our programme counter
	}
}

/**
 * /brief Jumps to the specified address if the carry flag is set. 
 * 
 * Sets the programme counter to the supplied address conditional upon the 
 * carry flag being set. As with all jumps it is important to note that the second 
 * address is big-endian while the programme's own memory interface is little endian.
 * Thus the MSB and LSB must be swapped. 
 * 
 * This function implements the following opcodes:
 *		DA
 *
 * @param programe_counter: Pointer to the programme counter
 * @param address_little_endian: The new value of the programme counter. Big endian.
 * @param flags: Pointer to the flags register
 */
void jump_carry_set(Register16 *programme_counter, unsigned short address_big_endian, Register8 *flags) {
	unsigned short address = to_little_endian(address_big_endian);
	
	// Mask out everything but the Carry Flag. 
	if (carry_flag_set(flags)) {
		*programme_counter = address;		// Update our programme counter
	}
}

/**
 * /brief Jumps the programme counter to the value stored in the indirect addressing 
 * register.
 * 
 * Jumps the programme counter to the value stored in the indirect addressing 
 * register. Unlike the other jumps, the address stored in this register is little-endian
 * so no conversion is necessary.
 * 
 * This function implements the following opcodes: 
 * 		E9
 *
 * @param programme_counter: Pointer to the programme counter. 
 * @param address_register: Pointer to the address register. The value in this 
 * 		register will be the new value of the programme counter.
 */
void jump_indirect(Register16 *programme_counter, Register16 *address_register) {
	*programme_counter = *address_register;
}

/**
 * /brief Performs a jump to a position relative to the current value of the programme counter
 *
 * Performs a jump to a position relative to the current position of the programme 
 * counter.
 * 
 * This function implements the following opcodes:
 * 		18
 *
 * @param programme_counter: Pointer to the programme counter.
 * @param offset: The amount we will add to our programme counter to get 
 * 		our new PC value
 */
void jump_relative_pos(Register16 *programme_counter, unsigned char offset) {
	*programme_counter += offset;
}

/**
 * /brief Performs a jump to a position relative to the current programme counter.
 *		The jump is only made if the Zero flag is not set.
 *
 * Performs a jump to a position relative to the current programme counter.
 *		The jump is only made if the Zero flag is not set.
 *
 * This function implements the following opcodes: 
 * 		20
 *
 * @param programme_counter: Pointer to the programme counter
 * @param offset: The amount we will add to the programme coiunter to get the 
 *		our new value.
 * @param flags: Pointer to the flags register.
 */
void jump_relative_zero_reset(Register16 *programme_counter, unsigned char offset, Register8 *flags) {
	// Flip bits, mask for zero
	if (!zero_flag_set(flags)) {
		*programme_counter += offset;
	}
}

/**
 * /brief Performs a jump to a position relative to the current programme counter.
 *		The jump is only made if the Zero flag is set.
 *
 * Performs a jump to a position relative to the current programme counter.
 *		The jump is only made if the Zero flag is set.
 *
 * This function implements the following opcodes:
 * 		28
 * 
 * @param programme_counter: Pointer to the programme counter
 * @param offset: The amount we will add to the programme coiunter to get the 
 *		our new value.
 * @param flags: Pointer to the flags register.
 */
void jump_relative_zero_set(Register16 *programme_counter, unsigned char offset, Register8 *flags) {
	// Flip bits, mask for zero
	if (zero_flag_set(flags)) {
		*programme_counter += offset;
	}
}

/**
 * /brief Performs a jump to a position relative to the current programme counter.
 *		The jump is only made if the carry flag is not set.
 *
 * Performs a jump to a position relative to the current programme counter.
 *		The jump is only made if the carry flag is not set.
 *
 * This function implements the following opcodes
 *		30
 *
 * @param programme_counter: Pointer to the programme counter
 * @param offset: The amount we will add to the programme coiunter to get the 
 *		our new value.
 * @param flags: Pointer to the flags register.
 */
void jump_relative_carry_reset(Register16 *programme_counter, unsigned char offset, Register8 *flags) {
	// Flip bits, mask for carry
	if (!carry_flag_set(flags)) {
		*programme_counter += offset;
	}
}

/**
 * /brief Performs a jump to a position relative to the current programme counter.
 *		The jump is only made if the carry flag is set.
 *
 * Performs a jump to a position relative to the current programme counter.
 *		The jump is only made if the carry flag is set.
 *
 * This function implements the following op codes:
 *		38
 *
 * @param programme_counter: Pointer to the programme counter
 * @param offset: The amount we will add to the programme coiunter to get the 
 *		our new value.
 * @param flags: Pointer to the flags register.
 */
void jump_relative_carry_set(Register16 *programme_counter, unsigned char offset, Register8 *flags) {
	// Flip bits, mask for carry
	if (carry_flag_set(flags)) {
		*programme_counter += offset;
	}
}



/***  CALLS ***/

/**
 * /brief Calls a function at the specified address
 *
 * Handles all of the book keeping needed to call a function. This includes saving the current 
 * value of the programme_counter on the stack and decrementing the stack pointer appropriately.
 * The supplied call address is loaded into the programme counter, and the function begins from the 
 * beginning of the new routine.
 *
 * This function implements the following opcodes: 
 * 		CD
 *
 * @param stack_pointer: Pointer to the stack pointer.
 * @param programme_counter: Pointer to the programme counter. At the beginning of execution, it will have 
 * the address of the CALL instruction. At the end it will be plaved at what ever address is supplied by 
 * call_address_big_endian.
 * @param call_address_big_endian: This is the address of the function we wish to begin executing. This 
 * address is supplied to us in a big endian format so some rearranging has to be done to make it compatable
 * with C which assumes numbers are little endian. 
 */
void call(Register16 *stack_pointer, Register16 *programme_counter, unsigned short call_address_big_endian) {
	push(stack_pointer, programme_counter);
	
	unsigned short call_address = to_little_endian(call_address_big_endian);

	// Load Call address to programme counter And we are good to go.
	*programme_counter = call_address;
}

/**
 * /brief: Calls a function at supplied address if zero flag is not set.
 * 
 * If the zero flag is reset, execution will begin at the address supplied 
 * in the call_address_big_endian parameter. Otherwise execution will continue
 * at the current location of the programme counter. 
 * 
 * This function implements the following opcodes:
 * 		C4
 * 
 * @param stack_pointer: Pointer to the stack pointer.
 * @param programme_counter: Pointer to the programme counter. At the beginning of execution, it will have 
 * the address of the CALL instruction. At the end it will be plaved at what ever address is supplied by 
 * call_address_big_endian.
 * @param call_address_big_endian: This is the address of the function we wish to begin executing. This 
 * address is supplied to us in a big endian format so some rearranging has to be done to make it compatable
 * with C which assumes numbers are little endian.
 * @param flags: Pointer to the flags register
 */ 
void call_zero_reset(Register16 *stack_pointer, Register16 *programme_counter, unsigned short call_address_big_endian, Register8 *flags) {
	if (!zero_flag_set(flags)) {
		call(stack_pointer, programme_counter, call_address_big_endian);
	}
}

/**
 * /brief: Calls a function at supplied address if zero flag is set.
 * 
 * If the zero flag is set, execution will begin at the address supplied 
 * in the call_address_big_endian parameter. Otherwise execution will continue
 * at the current location of the programme counter. 
 * 
 * This function implements the following opcodes:
 * 		CC
 * 
 * @param stack_pointer: Pointer to the stack pointer.
 * @param programme_counter: Pointer to the programme counter. At the beginning of execution, it will have 
 * the address of the CALL instruction. At the end it will be plaved at what ever address is supplied by 
 * call_address_big_endian.
 * @param call_address_big_endian: This is the address of the function we wish to begin executing. This 
 * address is supplied to us in a big endian format so some rearranging has to be done to make it compatable
 * with C which assumes numbers are little endian.
 * @param flags: Pointer to the flags register
 */ 
void call_zero_set(Register16 *stack_pointer, Register16 *programme_counter, unsigned short call_address_big_endian, Register8 *flags) {
	if (zero_flag_set(flags)) {
		call(stack_pointer, programme_counter, call_address_big_endian);
	}
}

/**
 * /brief: Calls a function at supplied address if carry flag is not set.
 * 
 * If the carry flag is reset, execution will begin at the address supplied 
 * in the call_address_big_endian parameter. Otherwise execution will continue
 * at the current location of the programme counter. 
 * 
 * This function implements the following opcodes:
 * 		D4
 * 
 * @param stack_pointer: Pointer to the stack pointer.
 * @param programme_counter: Pointer to the programme counter. At the beginning of execution, it will have 
 * the address of the CALL instruction. At the end it will be plaved at what ever address is supplied by 
 * call_address_big_endian.
 * @param call_address_big_endian: This is the address of the function we wish to begin executing. This 
 * address is supplied to us in a big endian format so some rearranging has to be done to make it compatable
 * with C which assumes numbers are little endian.
 * @param flags: Pointer to the flags register
 */ 
void call_carry_reset(Register16 *stack_pointer, Register16 *programme_counter, unsigned short call_address_big_endian, Register8 *flags) {
	if (!carry_flag_set(flags)) {
		call(stack_pointer, programme_counter, call_address_big_endian);
	}
}

/**
 * /brief: Calls a function at supplied address if carry flag is set.
 * 
 * If the carry flag is set, execution will begin at the address supplied 
 * in the call_address_big_endian parameter. Otherwise execution will continue
 * at the current location of the programme counter. 
 * 
 * This function implements the following opcodes:
 * 		DC
 * 
 * @param stack_pointer: Pointer to the stack pointer.
 * @param programme_counter: Pointer to the programme counter. At the beginning of execution, it will have 
 * the address of the CALL instruction. At the end it will be plaved at what ever address is supplied by 
 * call_address_big_endian.
 * @param call_address_big_endian: This is the address of the function we wish to begin executing. This 
 * address is supplied to us in a big endian format so some rearranging has to be done to make it compatable
 * with C which assumes numbers are little endian.
 * @param flags: Pointer to the flags register
 */ 
void call_carry_set(Register16 *stack_pointer, Register16 *programme_counter, unsigned short call_address_big_endian, Register8 *flags) {
	if (carry_flag_set(flags)) {
		call(stack_pointer, programme_counter, call_address_big_endian);
	}
}



/*** RESTARTS ***/

/**
 * /brief Pushes current prgramme counter to stack before restarting 
 * execution at 0x0000 + offset
 * 
 * The current value of the programme counter is pushed to the stack. The programme 
 * then begins executing from the memory address 0x0000 + offset. These offsets correspond to 
 * the locations of different interrupt vectors within the Z80GB memory space.  
 * 
 * This function implements the following opcodes
 * 		C7, CF, D7, DF, E7, EF, F7, FF
 * 
 * @param stack_pointer: Pointer to the stack pointer.
 * @param programme_counter: Pointer to the programme counter. At the beginning of execution, it will have 
 * the address of the RESTART instruction. At the end it will be placed at the address 0x0000 + offset. 
 * call_address_big_endian.
 * @param offset: The offset from 0x0000 the programme will resume execution at. 
 */
void restart(Register16 *stack_pointer, Register16 *programme_counter, unsigned char offset) {
	push(stack_pointer, programme_counter);

	*programme_counter = offset;	// The offset is from 0x0000 so we can simply supply the offset.
}

/*** RETURNS ***/

/**
 * /brief Unconditional function return.
 * 
 * Pops two bytes from the stack and sets the programme counter to that address. Execution
 * then begins at that address.
 * 
 * 
 * This function implements the following opcodes:
 * 		C9, D9
 * 
 * @param stack_pointer: Pointer to the stack pointer.
 * @param programme_counter: Pointer to the programme counter 
 */
void return_unconditional(Register16 *stack_pointer, Register16 *programme_counter) {
	pop(stack_pointer, programme_counter);
}

/**
 * /brief Returns from a function if the Zero Flag is not Set
 * 
 * Pops two bytes from the stack and sets the programme counter to that address. We do this 
 * only if the zero flag is not set.
 * 
 * This function implements the following opcodes:
 * 		C0
 * 
 * @param stack_pointer: Pointer to the SP register.
 * @param programme_counter: Pointer to the programme counter 
 * @param flags: Pointer to the flags register
 */
void return_zero_reset(Register16 *stack_pointer, Register16 *programme_counter, Register8 *flags) {
	if (!zero_flag_set(flags)) {
		pop(stack_pointer, programme_counter);
	}
}

/**
 * /brief Returns from a function if the Zero Flag is set
 * 
 * Pops two bytes from the stack and sets the programme counter to that address. We do this 
 * only if the zero flag is set.
 * 
 * This function implements the following opcodes:
 * 		C8
 * 
 * @param stack_pointer: Pointer to the SP register.
 * @param programme_counter: Pointer to the programme counter 
 * @param flags: Pointer to the flags register
 */
void return_zero_set(Register16 *stack_pointer, Register16 *programme_counter, Register8 *flags) {
	if (zero_flag_set(flags)) {
		pop(stack_pointer, programme_counter);
	}
}

/**
 * /brief Returns from a function if the carry Flag is not Set
 * 
 * Pops two bytes from the stack and sets the programme counter to that address. We do this 
 * only if the carry flag is not set.
 * 
 * This function implements the following opcodes:
 * 		D0
 * 
 * @param stack_pointer: Pointer to the SP register.
 * @param programme_counter: Pointer to the programme counter 
 * @param flags: Pointer to the flags register
 */
void return_carry_reset(Register16 *stack_pointer, Register16 *programme_counter, Register8 *flags) {
	if (!carry_flag_set(flags)) {
		pop(stack_pointer, programme_counter);
	}
}

/**
 * /brief Returns from a function if the carry Flag is set
 * 
 * Pops two bytes from the stack and sets the programme counter to that address. We do this 
 * only if the carry flag is set.
 * 
 * This function implements the following opcodes:
 * 		D8
 * 
 * @param stack_pointer: Pointer to the SP register.
 * @param programme_counter: Pointer to the programme counter 
 * @param flags: Pointer to the flags register
 */
void return_carry_set(Register16 *stack_pointer, Register16 *programme_counter, Register8 *flags) {
	if (carry_flag_set(flags)) {
		pop(stack_pointer, programme_counter);
	}
}

/*** MISC ***/

/**
 * /brief Swaps the upper and lower nibble of a value in a register
 * 
 * Swaps the upper and lower nibbles of a value stored within an 8 bit
 * register.
 * This function implements the following opcodes:
 *		37, 30, 31, 32, 33, 34, 35,
 *
 * @param target_register: The register containing the value we 
 * 		are doing a nibble swap for
 * @param flags: Pointer to the flags register
 */
void swap_nibble_register(Register8 *target_register, Register8 *flags) {
	unsigned char result = (*target_register >> 4) | (*target_register << 4);
	*flags = (result == 0) << 7;

	// Store result in the target register. 
	*target_register = result;
}
/**
 * /brief Swaps the upper and lower nibble of a value accessed
 * 		through indirect addressing,
 * Swaps the upper and lower nibbles of a value found at the memory address
 * contained in the HL address register.
 * This function implements the following opcodes:
 *		36
 *
 * @param target_register: The register containing memory address containing the
 *		value we are doing the swap for. 
 * @param flags: Pointer to the flags register
 */
void swap_nibble_indirect(Register16 *address_register, Register8 *flags) {
	unsigned char value_at_address = read_byte(*address_register);
	unsigned char result = (value_at_address >> 4) | (value_at_address << 4);
	*flags = (result == 0) << 7;

	// Store the result back in memory
	write_byte(*address_register, result);
}

/**
 * /brief Concerts value in accumulator to Binary Coded Decimal
 *
 * This operation converts the value in the accumulator into a
 * binary coded decimal. (Gosh, who EVER thought this was a sound 
 * idea).
 * To be honest, I'm not 100% sure if this is a correct implementation
 * as there is very little in the way of sound documentation for this 
 * functionality on the z80GB processor. For tractability, it should be
 * noted that I followed along with this explination of the operation:
 * http://sgate.emt.bme.hu/patai/publications/z80guide/app1b.html
 *
 * This operation implements the following opcodes:
 * 		27
 *
 * @param accumulator: Pointer to the accumulator
 * @param flags: Pointer to the flags register
 */
void decimal_adjust_accumulator(Register8 *accumulator, Register8 *flags) {
	unsigned char new_flags = *flags & 0x40;		// Operation preserves the N bit

	// First we deal with the lower nibble of our value=
	if ((*flags & 0x20) || (0x0F & *accumulator) > 0x09) {
		*accumulator += 0x06;	// Read as 0$06
	}

	// Now we deal with the upper nibble
	if ((*flags & 0x10) || (0xF0 & *accumulator) > 0x90) {
		*accumulator += 0x60;	// Read as 0$60
		new_flags |= 0x10;		// Per specifications, turn on carry bit
	}

	// Now we set our flags
	new_flags |= ((*accumulator == 0) << 7);
	*flags = new_flags;
}

/**
 * /brief Complements the value found in the accumulator
 *
 * Complements the value in the accumulator. The value will
 * thus feel a bit better about itself!
 * This operation implements the following opcodes:
 * 		2F
 *
 * @param *accumulator: Pointer to the accumulator register
 */
void complement_accumulator(Register8 *accumulator, Register8 *flags) {
	*accumulator = ~(*accumulator);
	*flags = 0x60;				// Flags set according to Z80gb specifications
}

/**
 * /brief Complements the carry bit in the flags register.
 *
 * Complements carry flag. The other flags will feel jealous.
 * This operation implements the following opcodes:
 * 		3F
 *
 * @param *accumulator: Pointer to the flags register
 */
void complement_carry_flag(Register8 *flags) {
	*flags &= 0x90;		// Clear the N & H bits
	*flags ^= 0x10;		// Toggle the carry bit.
}


/**
 * /brief Unconditionally sets the carry flag.
 *
 * Sets the carry flag.
 * This operation implements the following opcodes:
 * 		37
 *
 * @param *accumulator: Pointer to the flags register
 */
void set_carry_flag(Register8 *flags) {
	*flags &= 0x90;		// Reset N & H bits
	*flags |= 0x10;		// Set the carry flag
}

