/**
 * Game Boy (working title)
 * An emulator for a Game Boy.
 *
 * This module contains the function declaratiins necessary for loading a 
 * cartridge ROM into memory for a Game Boy emulator. 
 *
 * @author Rocky Petkov
 */

#ifndef CART_H
#define CART_H

#include <stdio.h>

// Addresses regarding where to find metadata on the cart. 
#define GAME_NAME_ADDRESS 			0x134
#define COLOUR_GB_FLAG_ADDRESS		0x143 
#define SUPER_GB_FLAG_ADDRESS 		0x146
#define CART_TYPE_ADDRESS 			0x147
#define ROM_SIZE_ADDRESS 			0x148
#define RAM_SIZE_ADDRESS 			0x149

#define NAME_LENGTH 				0x11

#define COLOUR_GB_FLAG 				0x80 	// If 0x80, the cart is for GBC
#define SUPER_GB_FLAG 				0x03 	// If 0x01, cart has Super GB features

#define ROM_BANK_SIZE 				16000	// Each bank of ROM is 16 KB

// Cart types!
#define ROM_ONLY 					0x00
#define ROM_MBC1 					0x01
#define ROM_MBC1_RAM 				0x02
#define ROM_MBC1_RAM_BATT   		0x03
#define ROM_MBC2            		0x05
#define ROM_MBC2_BATTERY			0x06
#define ROM_RAM 					0x08
#define ROM_RAM_BATTERY    			0x09
#define ROM_MMM01 					0x0B
#define ROM_MMM01_SRAM      		0x0C
#define ROM_MMM01_SRAM_BATT 		0x0D
#define ROM_MBC3_TIMER_BATT 		0x0F
#define ROM_MBC3_TIMER_RAM_BATT		0x10 
#define ROM_MBC3 					0x11
#define ROM_MBC3_RAM 				0x12
#define ROM_MBC3_RAM_BATT 			0x13
#define ROM_MBC5 					0x19
#define ROM_MBC5_RAM 				0x1A
#define ROM_MBC5_RAM_BATT 			0x1B 
#define ROM_MBC5_RUMBLE 			0x1C
#define ROM_MBC5_RUMBLE_SRAM 		0x1D
#define ROM_MBC5_RUMBLE_SRAM_BATT 	0x1E
#define ROM_POCKET_CAMERA 			0x1F 
#define ROM_BANDAI_TAMA5 			0xFD 
#define ROM_HUDSON_HUC3 			0xFE
#define ROM_HUDSON_HUC1				0xFF

/**
 * This little struct captures the relevant metadata
 * about a cartridge that we might seek to have available 
 * to us when we are reading a ROM file. 
 */
typedef struct {
	char *game_name;	
	unsigned char cart_type;/** Type of cartridge */
	int ram_size;			/** Size of RAM module on the cart in bytes */
	int rom_size; 			/** Size of ROM module on the cart in bytes */
	unsigned char super_gb_flag;	/** Indicates whether there are special super game boy functions */
	unsigned char colour_gb_flag;	/** Indicates whether the game is for the GBC. */
} CartMetaData;

// Some functions. See comments in rom.c for definitions
CartMetaData* load_rom(const char *file_location, unsigned char *memory_space);
CartMetaData* read_cart_metadata(FILE *rom_file);
int get_rom_size(unsigned char rom_byte);
int get_ram_size(unsigned char ram_byre);
void print_cart_metadata(CartMetaData *cart_data);
void free_cart_metadata(CartMetaData *cart_data)

#endif // CART_H