#include "../header/graph.h"

static int vertex_compare(void *o1, void *o2){
	if(o1 == NULL && o2 == NULL){return 0;}
	if(o1 == NULL){return 1;}
	if(o2 == NULL){return -1;}
	if(o1 == o2){return 0;}
	if(o1 > o2){return 1;}
	else{return -1;}
}
static int edge_compare(void *o1, void *o2){
	if(o1 == NULL && o2 == NULL){return 0;}
	if(o1 == NULL){return -1;}
	if(o2 == NULL){return 1;}
	if(o1 == o2){return 0;}
	if(o1 > o2){return 1;}
	else{return -1;}
}
static Edge* edge_constructor(int key, Vertex* v1, Vertex* v2, int weight){
	Edge* edge = malloc(sizeof(Edge));
	edge->key = key;
	edge->v1 = v1;
	edge->v2 = v2;
	edge->weight = weight;
	return edge;
}
static Vertex* vertex_constructor(int key, GRAPH_TYPE value){
	Vertex* vertex = malloc(sizeof(Vertex));
	vertex->key = key;
	vertex->value = value;
	vertex->edges = vector_constructor();
	vertex->paths = tree_map_constructor(&vertex_compare);
	vertex->distances = tree_map_constructor(&vertex_compare);
	return vertex;
}
Graph* graph_constructor(){
	Graph* graph = malloc(sizeof(Graph));
	graph->vertex_list = vector_constructor();
	graph->edge_list = vector_constructor();
	return graph;
}
Edge* add_edge(int key, Vertex* v1, Vertex* v2, int weight, Graph* graph){
	// need an assert statement here so cannot create multiple edges between vertices.
	assert(v1 != NULL && v2 != NULL);
	Edge* edge = edge_constructor(key, v1, v2, weight);
	vector_add(edge, v1->edges);
	vector_add(edge, v2->edges);
	vector_add(edge, graph->edge_list);
	Dijkstra(v1, graph);
	Dijkstra(v2, graph);
	return edge;
}
Vertex* add_vertex(int key, GRAPH_TYPE value, Graph* graph){
	Vertex *vertex = vertex_constructor(key, value);
	vector_add(vertex, graph->vertex_list);
	return vertex;
}

void print_vertex(Vertex *vertex){
	printf("Vertex key#: %d\n", vertex->key);
	printf("Connected Edges (%d):\n", vertex_edge_count(vertex));
	int edgeCounter = 0;
	for(edgeCounter = 0; edgeCounter < vertex_edge_count(vertex); edgeCounter++){
		Edge* e = vector_get(edgeCounter, vertex->edges);
		print_edge(e);
	}
}

void print_edge(Edge *edge){
	printf("Edge key#: %d, v1: %d, v2: %d\n", edge->key, edge->v1->key, edge->v2->key);
}

int vertex_edge_count(Vertex* vertex){
	vector_size(vertex->edges);
}

int graph_edge_count(Graph *graph){
	vector_size(graph->edge_list);
}

int graph_vertex_count(Graph *graph){
	vector_size(graph->vertex_list);
}

Edge* edge_between(Vertex* source, Vertex *target){
	int target_counter;
	int source_counter;
	for(target_counter = 0; target_counter < vertex_edge_count(target); target_counter++){
		for(source_counter = 0; source_counter < vertex_edge_count(source); source_counter++){
			if(vector_get(source_counter, source->edges) == vector_get(target_counter, target->edges)){
				return vector_get(source_counter, source->edges);
			}
		}
	}
	return NULL;
}

void BredthFirstTraversal(Vertex *start){
	Queue* vertices = queue_constructor();
	TreeSet* visited = tree_set_constructor(&vertex_compare);
	queue_push(start, vertices);
	tree_set_put(start, visited);
	while(queueIsEmpty(vertices) == 0){
		Vertex* front = queue_pop(vertices);
		Iterator* adjacent_vertices_iterator = adjacent_vertices(front);
		while(iterator_next(adjacent_vertices_iterator)) {
			Vertex *next = iterator_next(adjacent_vertices_iterator);
			if(tree_set_contains(next, visited) == 0){
				queue_push(next, vertices);
				tree_set_put(next, visited);
			}
		}
		printf("%d\n", front->key);
	}
}

// pretty sure this works as depth first traversal. before we did the hasUnvisitedKey thing, but i do not think this is necessary.
// do it out in ur head and ull see. it keeps pushing on top so it will act as depth first. since it will keep visiting children on the one path.
/*
1  procedure DFS-iterative(G,v):
2      let S be a stack
3      S.push(v)
4      while S is not empty
5            v = S.pop() 
6            if v is not labeled as discovered:
7                label v as discovered
8                for all edges from v to w in G.adjacentEdges(v) do
9                    S.push(w)
*/
void DepthFirstTraversal(Vertex *start){
	Stack* vertices = stack_constructor();
	TreeSet* visited = tree_set_constructor(&vertex_compare);
	stack_push(start, vertices);
	tree_set_put(start, visited);
	while(stackIsEmpty(vertices)==0){
		Vertex* top = stack_pop(vertices);
		Iterator* adjacent_vertices_iterator = adjacent_vertices(top);
		while(iterator_hasNext(adjacent_vertices_iterator)) {
			Vertex *next = iterator_next(adjacent_vertices_iterator);
			if(tree_set_contains(next, visited) == 0){
				stack_push(next, vertices);
				tree_set_put(next, visited);
			}
		}
		printf("%d\n", top->key);;
	}
}

static int fromPointer(void *ptr){
	return *((int*)ptr);
}

static int* toPointer(int value){
	int* newint = malloc(sizeof(int));
	*newint = value;
	return newint;
}

static PriorityQueue* createVertexQueue(Vertex *start, Graph *graph){
	PriorityQueue* queue = priorityqueue_constructor(&vertex_compare);
	int counter;
	for(counter = 0; counter < graph_vertex_count(graph); counter++){
		Vertex* v = vector_get(counter, graph->vertex_list);
		priorityqueue_push(tree_map_get(start, v->distances), v, queue);
	}
	int* weight = toPointer(0);
	tree_map_put(start, weight, start->distances);
	priorityqueueSetKey(weight, start, queue);
	return queue;
}

/*
bool relax(Graph &g, int u, int v){
	Graph::vertex_descriptor v1 = u;
	Graph::vertex_descriptor v2 = v;

	Graph::edge_descriptor e = edge(v1, v2, g).first;
	if(g[v2].weight > g[v1].weight + g[e].weight)
	{
		g[v2].weight = g[v1].weight + g[e].weight;
		g[v2].pred = v1;
		return true;
	}
	return false;
}
*/

/*
static int relax(Vertex* source, Vertex *target){
	Edge* bridge = edge_between(front, next);
	int* weight = toPointer(fromPointer(tree_map_get(start, front->distances)) + bridge->weight);
	if(tree_map_contains(start, next->distances) == 0 || fromPointer(tree_map_get(start, next->distances)) > *weight){
		tree_map_put(start, weight, next->distances);
		return 1;
	}
	return 0;
}
*/

void Dijkstra(Vertex* start, Graph* graph){
	TreeSet* visited = tree_set_constructor(&vertex_compare);
	PriorityQueue* queue = createVertexQueue(start, graph);
	while(priorityqueueIsEmpty(queue)==0){
		Vertex* front = priorityqueue_pop(queue);
		tree_set_put(front, visited);
		Iterator* adjacent_vertices_iterator = adjacent_vertices(front);
		while(iterator_hasNext(adjacent_vertices_iterator)) {
			Vertex *next = iterator_next(adjacent_vertices_iterator);
			if(tree_set_contains(next, visited) == 0) {
				Edge* bridge = edge_between(front, next);
				int* weight = toPointer(fromPointer(tree_map_get(start, front->distances)) + bridge->weight);
				if(tree_map_contains(start, next->distances) == 0 || fromPointer(tree_map_get(start, next->distances)) > *weight){
					tree_map_put(start, weight, next->distances);
					priorityqueueSetKey(weight, next, queue);
				}
			}
		}
	}
}

Iterator* adjacent_vertices(Vertex *vertex){
	Iterator* vertices = iterator_constructor();
	int edge_counter = 0;
	for(edge_counter = 0; edge_counter < vertex_edge_count(vertex); edge_counter++){
		Edge *e = vector_get(edge_counter, vertex->edges);
		if(e->v1 != vertex){
			iterator_push(e->v1, vertices);
		}
		else{
			iterator_push(e->v2, vertices);
		}
	}
	return vertices;
}

void print_graph(Graph *graph){
	int vertexCounter = 0;
	for(vertexCounter = 0; vertexCounter < graph_vertex_count(graph); vertexCounter++){
		Vertex* v = vector_get(vertexCounter, graph->vertex_list);
		print_vertex(v);
	}
}

void printDistanceFrom(Vertex* from, Graph* graph){
	printf("Distance from: %d\n", from->key);
	int vertexCounter;
	for(vertexCounter = 0; vertexCounter < graph_vertex_count(graph); vertexCounter++){
		Vertex* next = vector_get(vertexCounter, graph->vertex_list);
		int* weight = tree_map_get(from, next->distances);
		if(weight != NULL){
			printf("%d: %d\n", next->key, fromPointer(weight));
		}
	}
}
