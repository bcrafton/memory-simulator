.PHONY: all
all: vpi compile run

.PHONY: vpi
vpi:
	iverilog-vpi func.c

.PHONY: compile
compile:
	iverilog -ofunc.vvp func.v

.PHONY: run
run:
	vvp -M. -mfunc func.vvp

.PHONY: clean
clean:
	rm -f *.vvp *.vpi *.o
