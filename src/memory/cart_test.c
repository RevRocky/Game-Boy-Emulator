/*
 * A little test programme one can leverage to ensure that the functionality pertaining to 
 * interfacing with the cartridge is working correctly. 
 */

#include <stdio.h>
#include <stdlib.h>

#include "cart.h"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Invalid Number of Arguments.\n");
		exit(1);
	}

	char *rom_location = argv[1];

	// Initialise memory
	unsigned char *memory = calloc(64000, sizeof(unsigned char));
	load_rom(rom_location, memory);

}