/**
 * A header file containing functions and other helpful tools for when the programme
 * is working with memory.
 *
 * Authors: Rocky Petkov
 */

#ifndef MEMORY_H
#define MEMORY_H

#define IO_PORT_MEMORY_BASE 0xFF00

extern unsigned char *memory_space;		// We'd like access to system memory. It might be useful


// See memory.c for more thorough explination of these functions 
unsigned char read_byte(unsigned short address);
void write_byte(unsigned short address, unsigned char byte);

#endif