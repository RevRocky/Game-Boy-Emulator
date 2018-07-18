/**
 * Game Boy (working title)
 * An emulator for a Game Boy.
 *
 * This module contains the functionality necessary for loading a cartridge ROM into memory for 
 * a Game Boy emulator. 
 *
 * @author Rocky Petkov
 */

#include "cart.h"

#include <stdlib.h>

/**
 * /brief Loads a ROM into the Game Boy's memory space. 
 * 
 * Loads the ROM file located at the given file location into the 
 * memory space for the Game Boy. For the time being, this function
 * assumes no funny business concerning the size of the ROM. Basically, 
 * we expect the entire game to fit in the perscribed 32KB space. If, for some reason
 * it is larger, then we will only read the first 32 kb.
 *
 * @param file_location: Location of the ROM file on disk.
 * @param memory_space: Pointer to the array used to represent the Game Boy's memory.
 *	The values will be updated in place and not explicitly returned. This IS C afterall!
 *
 * @return Number of elements successfully read from the rom... or the error code of the 
 *  read operation
 */
CartMetaData* load_rom(const char *file_location, unsigned char *memory_space) {
	CartMetaData *cart_data;

	// Debug output if we want it!
	#ifdef VERBOSE 
		printf("Opening ROM File....");
	#endif

	// Open, read, store, close, return!
	FILE *rom_file = fopen(file_location, "rb");
	if (rom_file == NULL) {
		perror("Error Opening File");
		exit(2);
	}	

	// Since behaviour depends upon the type of cart we're emulating, we have to know 
	// the size of the ROM file.
	fseek(rom_file, 0, SEEK_END);
	int file_size = ftell(rom_file);
	fseek(rom_file, 0, SEEK_SET);		// Reset the file pointer. 

	// We don't actually need the contents of the cart. Just the metadata should suffice for now.
	cart_data = read_cart_metadata(rom_file);

	//TODO: Implement Alternative Cart Types Here!
	// For the time being we're only going to impliment 32KB ROM only carts like Tetris. 
	// NOTE: The cart type for the ROM only cart is 0x00.
	if (!cart_data->cart_type) {
		perror("The emulator does not currently have support for fancy pants cartridges. Come back later!");
		exit(3);
	}
	// If we have a usable cart, copy data into memory. 
	fseek(rom_file, 0, SEEK_SET);
	unsigned bytes_read = fread(memory_space, sizeof(unsigned char), cart_data->rom_size, rom_file);

	#ifdef VERBOSE
		printf("Read %d bytes from the ROM", bytes_read);
	#endif

	return cart_data;
}

/**
 * /brief Reads metadata from the catridge ROM.
 *
 * Reads a collection of metadata including game name, cart_type,
 * memory sizes as well as other pertinent information from the cartridge.
 * This will allow the emulator to assess whether it is capable of running the 
 * file. 
 *
 * @param rom_file: Pointer to the rom file on disk.
 *
 * @return A CartMetaData struct with information about the rom!
 */
CartMetaData* read_cart_metadata(FILE *rom_file) {
	CartMetaData *cart_data = calloc(1, sizeof(CartMetaData));
	char *cart_name = calloc(NAME_LENGTH, sizeof(char));

	// Define some more pointers to be used as buffers
	unsigned char *colour_gb_ptr = malloc(sizeof(unsigned char));
	unsigned char *super_gb_ptr = malloc(sizeof(unsigned char));
	unsigned char *cart_type_ptr = malloc(sizeof(unsigned char));
	unsigned char *rom_size_ptr = malloc(sizeof(unsigned char));
	unsigned char *ram_size_ptr = malloc(sizeof(unsigned char));


	// For cleanliness we will read all values to buffers, transfer to 
	// struct and then tidy up the pointers!

	// Read the name of game
	fseek(rom_file, GAME_NAME_ADDRESS, SEEK_SET);
	int bytes_read = fread(cart_name, sizeof(char), NAME_LENGTH, rom_file);
	cart_name[bytes_read] = '\0';							// Ensure the string is null terminated. 

	// Read the colour pointer. It is next after all!
	fread(colour_gb_ptr, sizeof(unsigned char), 1, rom_file);

	// Now we advance to the SGB flag
	fseek(rom_file, SUPER_GB_FLAG_ADDRESS, SEEK_SET);
	fread(super_gb_ptr, sizeof(unsigned char), 1, rom_file);

	// We can go on through just reading the cart type flags etc. Tney are back to back.
	fread(cart_type_ptr, sizeof(unsigned char), 1, rom_file);
	fread(rom_size_ptr, sizeof(unsigned char), 1, rom_file);
	fread(ram_size_ptr, sizeof(unsigned char), 1, rom_file);

	// Assigning the values to the struct
	cart_data->game_name 		= cart_name;
	cart_data->cart_type 		= *cart_type_ptr;
	cart_data->colour_gb_flag 	= *colour_gb_ptr == COLOUR_GB_FLAG;	
	cart_data->super_gb_flag    = *super_gb_ptr == SUPER_GB_FLAG;

	// The bytes indicating RAM and ROM size do not directly communicate
	// the size of memory banks. The following functions will get at that.
	cart_data->rom_size = get_rom_size(*rom_size_ptr);
	cart_data->ram_size = get_ram_size(*ram_size_ptr);

	// Free up our pointers.
	free(colour_gb_ptr);
	free(super_gb_ptr);
	free(cart_type_ptr);
	free(rom_size_ptr);
	free(ram_size_ptr);

	// Set the name ptr to null for safety.
	cart_name = NULL;
	return cart_data;
}

/**
 * /brief Gets the size of the ROM on Cartridge
 *
 * Returns the size of ROM on cartridge. The mapping is pretty straight forward
 * If the byte is between 0-6 then it's 2^(n+1) banks of ROM with each bank being 
 * 16KB of memory. 
 * For byte values other than 0-6 there's no real pattern, one can use:
 * http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf p. 12 as a reference.
 * Banks are still 16KB of memory though!
 * 
 * @param rom_byte: Byte on cart corresponding to the amount of ROM.
 *
 * @return The size of ROM in bytes.
 */
int get_rom_size(unsigned char rom_byte) {
	if (rom_byte <= 6) {
		return (2 << (rom_byte + 1)) * ROM_BANK_SIZE;
	}
	else if (rom_byte == 0x52) {
		return 72 * ROM_BANK_SIZE;
	}
	else if (rom_byte == 0x53) {
		return 80 * ROM_BANK_SIZE;
	}
	else if (rom_byte == 0x54) {
		return 96 * ROM_BANK_SIZE;
	}
	else {
		return -1;			// Faulty ROM read.
	}
}

/**
 * /brief Gets the size of the ROM on Cartridge
 *
 * Returns the size of RAM on cartridge, if any. There's no pattern to the 
 * bank size so if one wishes to see documentation, the link above should 
 * suffice as a method to this madness
 * 
 * @param ram_byte: Byte on cart corresponding to the amount of RAM.
 *
 * @return The size of RAM in bytes.
 */
int get_ram_size(unsigned char ram_byte) {
	switch(ram_byte) {
		case 0	:
			return 0;
		case 1	:
			return 2000;	// 2KB RAM
		case 2	: 
			return 8000;	// 8KB RAM
		case 3	:
			return 32000;	// 32KB RAM
		case 4	:
			return 128000;	// 128KB ram
		default :
			return -1;		// Faulty RAM read.
	}
}

/**
 * /brief Pretty Prints the cart metadata. 
 *
 * Prints the metadata for a cartridge in a nice, easy to read fashion!
 * 
 * @param cart_data: A CartMetaData Struct with the cart whose info is being printed.
 */
void print_cart_metadata(CartMetaData *cart_data) {
	printf("\n\n%s\n", cart_data->game_name);
	printf("\tCart Type: %x\n", cart_data->cart_type);
	printf("\tRam Size: %d\n", cart_data->ram_size);
	printf("\tRom Size: %d\n", cart_data->rom_size);
	printf("\tSuper GB: %d\tColour GB: %d\n", cart_data->super_gb_flag, cart_data->colour_gb_flag);
}

/**
 * /brief Removes Cartridge Metadata from memory.
 * 
 * Removes the metadata for a cartridge from memory. Remember to set the value of 
 * the 
 *
 * @param cart_data: Cart metadata that we wish to remove from memory
 */
void free_cart_metadata(CartMetaData *cart_data) {
	free(cart_data->game_name);
	free(cart_data);
}