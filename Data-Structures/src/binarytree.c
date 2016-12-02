#include "../header/binarytree.h"

static int compare(KEY_TYPE v1, KEY_TYPE v2, Binarytree *tree);
static int print(VALUE_TYPE v, Binarytree *tree);

BinarytreeNode* binarytree_node_constructor(KEY_TYPE key, VALUE_TYPE value) {
	BinarytreeNode *node = malloc(sizeof(BinarytreeNode));
	node->right = NULL;
	node->left = NULL;
	node->value = value;
	node->key = key;
	return node;
}

Binarytree* binarytree_constructor_print( void (*tree_print_function)(void*), int (*tree_compare_function)(void*, void*) ){
	Binarytree *tree = malloc(sizeof(Binarytree));
	tree->root = NULL;
	tree->tree_print_function = tree_print_function;
	tree->tree_compare_function = tree_compare_function;
	return tree;
}

Binarytree* binarytree_constructor( int (*tree_compare_function)(void*, void*) ){
	Binarytree *tree = malloc(sizeof(Binarytree));
	tree->root = NULL;
	tree->tree_print_function = NULL;
	tree->tree_compare_function = tree_compare_function;
	return tree;
}

void binarytree_add(KEY_TYPE key, VALUE_TYPE value, Binarytree* tree){
	BinarytreeNode* newNode = binarytree_node_constructor(key, value);
	tree->size++;
	
	if(tree->root == NULL){
		tree->root = newNode;
		return;
	}
	BinarytreeNode* itr = tree->root;
	while(itr != NULL){
		if( compare(key, itr->key, tree) > 0){
			if(itr->right == NULL){
				itr->right = newNode;
				return;
			}
			itr = itr->right;
		}
		else{
			if(itr->left == NULL){
				itr->left = newNode;
				return;
			}
			itr = itr->left;
		}
	}
}

VALUE_TYPE binarytree_search(KEY_TYPE key, Binarytree* tree){
	BinarytreeNode* itr = tree->root;
	while(itr != NULL){
		if( compare(key, itr->key, tree) == 0 ){
			return itr->value;
		}
		else if( compare(key, itr->key, tree) > 0 ){
			itr = itr->right;
		}
		else{
			itr = itr->left;
		}
	}
	return NULL;
}

void binarytree_stack_print(Binarytree *tree){
	Stack *stack = stack_constructor();
	stack_push(tree->root, stack);
	while(stackIsEmpty(stack) == 0){
		BinarytreeNode* top = stack_pop(stack);
		if(top->left != NULL){
			stack_push(top->left, stack);
		}
		if(top->right != NULL){
			stack_push(top->right, stack);
		}
		print(top->value, tree);
		printf("|");
	}
	printf("[%d]\n", tree->size);
}

int binarytree_contains(KEY_TYPE key, Binarytree* tree){
	return binarytree_search(key, tree) != NULL;
}

static int compare(KEY_TYPE v1, KEY_TYPE v2, Binarytree *tree){
	return (*(tree->tree_compare_function))(v1, v2);
}

static int print(VALUE_TYPE v, Binarytree *tree){
	(*(tree->tree_print_function))(v);
}
