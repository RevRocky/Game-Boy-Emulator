# Variables

cpu_dir = src/cpu
memory_dir = src/memory

obj_dir = build/obj
test_exe_dir = build/test
emu_dir = build

$(test_exe_dir)/alutest : $(obj_dir)/alutest.o $(obj_dir)/instructions_test_alu.o $(obj_dir)/register_test_alu.o $(obj_dir)/memory_test_alu.o
	gcc -g -o $(test_exe_dir)alutest $(obj_dir)/alutest.o $(obj_dir)instructions_test_alu.o $(obj_dir)/register_test_alu.o $(obj_dir)/memory_test_alu.o

$(obj_dir)alutest.o : src/alutest.c 
	gcc -g -c src/alutest.c

$(obj_dir)/instructions_test_alu.o : $(cpu_dir)/instructions.c
	gcc -g -o $(cpu_dir)/instructions_test_alu.o -c $(cpu_dir)/instructions.c

$(obj_dir)/register_test_alu.o : $(cpu_dir)/register.c
	gcc -g -o $(obj_dir)/register_test_alu.o -c $(cpu_dir)/register.c

$(obj_dir)memory_test_alu.o : $(memory_dir)/memory.c 
	gcc -g -o $(obj_dir)/memory_test_alu.o -c $(memory_dir)/memory.c
