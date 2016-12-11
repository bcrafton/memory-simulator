
#include "memory_simulator.h"

int time_compare(void *o1, void *o2){
	return *((unsigned long*)o1) - *((unsigned long*)o2);
}
