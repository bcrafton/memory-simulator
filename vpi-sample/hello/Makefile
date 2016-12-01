.PHONY: all
all: vpi compile run

.PHONY: vpi
vpi:
	iverilog-vpi hello.c

.PHONY: compile
compile:
	iverilog -ohello.vvp hello.v

.PHONY: run
run:
	vvp -M. -mhello hello.vvp

.PHONY: clean
clean:
	rm -f *.vvp *.vpi *.o
