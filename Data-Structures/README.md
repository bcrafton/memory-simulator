# Data Structures
The repo implements a bunch of common data structures in C </br>
These could definitely be implemented better</br>
Some Problems:</br>
1. Use void* to make generic and does not take size of object, so can only pass and receive pointers</br>
2. Dont have the nice wrapper stuff that C allows</br>
### Compilation
This is written for gcc.

Example compilation: </br>
**gcc graph_test.c ../src/graph.c ../src/list.c ../src/vector.c ../src/heap.c ../src/queue.c ../src/stack.c ../src/tree_set.c ../src/rbtree.c ../src/tree_map.c ../src/priority_queue.c ../src/iterator.c**
### Running the Code
**./a.out 100**
