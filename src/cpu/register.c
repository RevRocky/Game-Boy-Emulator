/*
 * Game Boy (working title)
 * An emulator for a Game Boy.
 *
 * This file contains functionality pertaining directly to initialising,
 * and interacting with the registers in an emulated Game Boy. 
 * Authors: Rocky Petkov
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "register.h"

/**
 * /brief Initialises CPU state
 *
 * Initialises the set of registers to a known state for when the
 * "Game Boy" turns on. This involves setting all 8 bit registers to 
 * 0 while the Stack Pointer is intiialised to 0xE000 and the Programme 
 * Counter is initalised to 0x100.
 *
 * @return A pointer to a brand new "CPU State" struct
 */
CPUState* initialise_registers() {
	// Since we've cleared and intialised, everything will be 0!
	CPUState* new_state = calloc(1, sizeof(CPUState));
	new_state->SP = 0xE000;
	new_state->PC = 0x100;

	return new_state;
}

/**
 * /brief Prints the current values of the registers
 * 
 * Prints the current values of the registers
 */
void print_registers(CPUState* system_state) {
	// Get current time so we can print it!
	time_t raw_time;
	struct tm * time_info;

	time(&raw_time);	// Acquiring the current time!
	time_info = localtime(&raw_time);

	printf("CPU State @ Time: %s", asctime(time_info));				// Gives context to the print out.	
	printf("\tA: %X\tF: %X\n", (system_state->A), (system_state->F));
	printf("\tB: %X\tC: %X\n", (system_state->B), (system_state->C));
	printf("\tD: %X\tE: %X\n", (system_state->D), (system_state->E));
	printf("\tH: %X\tL: %X\n", (system_state->H), (system_state->L));
	printf("\tStack Pointer; %X\n", (system_state->SP));
	printf("\tProgramme Counter: %X\n\n", (system_state->PC));
} 
