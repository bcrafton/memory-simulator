#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/graph.h"
#include "../header/vector.h"
#include <assert.h>

void vertex_print_function(void *v){
	print_vertex((Vertex*)v);
}
void int_print(void *s){
	printf("%d", *((int*)s));
}
void cstring_print(void *s){
	printf("%s", (char*)s);
}
int int_compare(void *o1, void *o2){
	if(o1 == NULL && o2 == NULL){
		return 0;
	}
	if(o1 == NULL){
		return 1;
	}
	if(o2 == NULL){
		return -1;
	}
	return *((int*)o1) - *((int*)o2);
}
int cstring_compare(void *o1, void *o2){
	return strcmp((char*)o1, (char*)o2);
}
int* get_int_ptr(int value){
	int* temp = malloc(sizeof(int));
	*temp = value;
	return temp;
}

int main(){
	char* a1 = "Brian";
	char* b1 = "Julia";
	char* c1 = "Rex";

	Graph* graph = graph_constructor();

	Vertex* v1 = add_vertex(0, a1, graph);
	Vertex* v2 = add_vertex(1, b1, graph);
	Vertex* v3 = add_vertex(2, c1, graph);

	//Edge* e = add_edge(0, NULL, v2, 10, graph);
	Edge* e1 = add_edge(0, v1, v2, 10, graph);
	Edge* e2 = add_edge(1, v2, v3, 15, graph);
	Edge* e3 = add_edge(2, v3, v1, 20, graph);

	/*
	printf("\n");
	print_graph(graph);

	printf("\n");
	BredthFirstTraversal(v1);
	printf("\n");
	DepthFirstTraversal(v1);
	printf("\n");
	*/

	printDistanceFrom(v1, graph);
	printDistanceFrom(v2, graph);
	printDistanceFrom(v3, graph);
}
