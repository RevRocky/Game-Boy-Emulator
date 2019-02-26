# Variables

cpu_dir = src/cpu
memory_dir = src/memory

obj_dir = build/obj
test_exe_dir = build/test
emu_dir = build

alu_test_dependencies = $(obj_dir)/instructions_test_alu.o $(obj_dir)/register_test_alu.o $(obj_dir)/memory_test_alu.o $(obj_dir)/util.o

$(test_exe_dir)/alutest : $(obj_dir)/alutest.o $(alu_test_dependencies)
	gcc -g -o $(test_exe_dir)/alutest $(obj_dir)/alutest.o $(alu_test_dependencies)

$(obj_dir)/alutest.o : src/alutest.c $(alu_test_dependencies)
	gcc -g -o $(obj_dir)/alutest.o -c src/alutest.c 

$(obj_dir)/util.o : src/util.c
	gcc -g -o $(obj_dir)/util.o -c src/util.c

$(obj_dir)/instructions_test_alu.o : $(cpu_dir)/instructions.c
	gcc -g -o $(obj_dir)/instructions_test_alu.o -c $(cpu_dir)/instructions.c

$(obj_dir)/register_test_alu.o : $(cpu_dir)/register.c
	gcc -g -o $(obj_dir)/register_test_alu.o -c $(cpu_dir)/register.c

$(obj_dir)/memory_test_alu.o : $(memory_dir)/memory.c 
	gcc -g -o $(obj_dir)/memory_test_alu.o -c $(memory_dir)/memory.c


clean: 
	rm build/obj/*.o