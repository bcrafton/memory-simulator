.PHONY: all
all: vpi compile run

.PHONY: vpi
vpi:
	iverilog-vpi task.c

.PHONY: compile
compile:
	iverilog -otask.vvp task.v

.PHONY: run
run:
	vvp -M. -mtask task.vvp

.PHONY: clean
clean:
	rm -f *.vvp *.vpi *.o
