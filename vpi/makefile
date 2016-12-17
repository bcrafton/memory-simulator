.PHONY: all
all: vpi compile run

.PHONY: vpi
vpi:
	iverilog-vpi memory_controller.c cache.c memory.c memory_simulator.c ../Data-Structures/src/list.c priority_list.c

.PHONY: compile
compile:
	iverilog -o memory_controller.vvp top.v memory_controller.v memory_controller.sft

.PHONY: run
run:
	vvp -M. -m memory_controller memory_controller.vvp

.PHONY: clean
clean:
	rm -f *.vvp *.vpi *.o *.out
