alutest : alutest.o cpu/instructions_test_alu.o cpu/register_test_alu.o memory/memory_test_alu.o
	gcc -g -o alutest alutest.o cpu/instructions_test_alu.o cpu/register_test_alu.o memory/memory_test_alu.o

alutest.o : alutest.c 
	gcc -g -c alutest.c

cpu/instructions_test_alu.o : cpu/instructions.c
	gcc -g -o cpu/instructions_test_alu.o -c cpu/instructions.c

cpu/register_test_alu.o : cpu/register.c
	gcc -g -o cpu/register_test_alu.o -c cpu/register.c

memory/memory_test_alu.o : memory/memory.c 
	gcc -g -o memory/memory_test_alu.o -c memory/memory.c
