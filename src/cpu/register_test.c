/* 
 * A little test progrmame to test the registers.
 */

# include "register.h"
# include <stdio.h>
# include <stdlib.h>

int main() {
	CPUState* system_state = initialise_registers();
	print_registers(system_state);

	system_state->A = 0x45;
	system_state->PC++;
	print_registers(system_state);
}