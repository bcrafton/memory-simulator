.PHONY: all
all: vpi compile run

.PHONY: vpi
vpi:
	iverilog-vpi memory_controller.c

.PHONY: compile
compile:
	iverilog -omemory_controller.vvp top.v memory_controller.v

.PHONY: run
run:
	vvp -M. -mmemory_controller memory_controller.vvp

.PHONY: clean
clean:
	rm -f *.vvp *.vpi *.o
