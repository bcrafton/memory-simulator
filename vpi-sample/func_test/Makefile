.PHONY: all
all: vpi compile run

.PHONY: vpi
vpi:
	iverilog-vpi func.c func.sft

.PHONY: compile
compile:
	iverilog -ofunc.vvp func.v func.sft

.PHONY: run
run:
	vvp -M. -mfunc func.vvp

.PHONY: clean
clean:
	rm -f *.vvp *.vpi *.o
