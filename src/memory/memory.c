/**
 * This module contains functions and other resources useful for interfacing with the system's 
 * memory.
 *
 * Authors: Rocky Petkov
 */

#include "memory.h"

/**
 * /brief Reads a byte of memory from the supplied address
 *
 * Reads a byte of memory from the supplied address. This function will not pay heed 
 * to whether the request is legal or even advisable, so it is best to ensure this in the
 * calling environment. 
 * 
 * @param address: Address of where the byte we wish to read
 * 
 * @return: The value stored at the supplied address.
 */
unsigned char read_byte(unsigned short address) {
	return memory_space[address];
}

/**
 * /brief Writes the supplied byte to a given address
 *
 * Writes a byte to memory at the supplied address. This function
 * pays no heed to whether the request is legal or advidable, so it is 
 * best to ensure legality within the calling environment
 *
 * @param address: The address we wish to write the byte to
 * @param byte: The byte we wish to write to memory
 */
void write_byte(unsigned short address, unsigned char byte) {
	memory_space[address] = byte;
}