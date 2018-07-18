/*
 * Game Boy (working title)
 * An emulator for a Game Boy.
 *
 * This is a header file containing definitions for the different data
 * structures that will be used to represent a register within the 
 * emulator. 
 * Authors: Rocky Petkov
 *
 */

#ifndef REGISTER_H
#define REGISTER_H 

// Defining positions for the status flags.
#define ZERO_FLAG_POS 			7
#define SUB_FLAG_POS  			6
#define HALF_CARRY_FLAG_POS		5
#define CARRY_FLAG_POS 			4

// Some typedefs for cleanliness. 
// Pay close attention to how a register is a pointer 
// to a value, not the value it self. 
typedef unsigned char Register8;	
typedef unsigned short Register16;	

/* 
 * This union allows us to easily handle two 
 * associated 8 bit registers within the Gameboy to 
 * treat them as a single 16 bit register.
 * 
 * For the time being, I've opted to use direct storage of 
 * values as opposed to using pointers as it results in a 
 * more compact and... hopefully easy to use structure. 
 */
typedef struct {

	// Defining the general purpose registers
	union {
		struct {
			Register8 A;	// In this case, A is the upper byte, F is the lower byte
			Register8 F;	// Do not directly access F.
		};
		Register16 AF;
	};

	union {
		struct {
			Register8 B;
			Register8 C;
		};
		Register16 BC;
	};

	union {
		struct {
			Register8 D;
			Register8 E;
		};
		Register16 DE;
	};

	union {
		struct {
			Register8 H;
			Register8 L;
		};
		Register16 HL;		// HL is often used as a 16 bit register for indirect addressing and such
	};

	Register16 SP;			// The stack pointer
	Register16 PC;			// The programme counter
	
} CPUState;


//And some functions. See register.c for definitions!

CPUState* initialise_registers();
void print_registers(CPUState* system_state);


#endif // REGISTER_H

