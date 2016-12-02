#include "../header/rbtree.h"

static int compare(KEY_TYPE v1, KEY_TYPE v2, RBTree *tree);
static int print(VALUE_TYPE v, RBTree *tree);
static void adjustAfterInsertion(RBTreeNode* n, RBTree* tree);
static int isRed(RBTreeNode* node);
static int isBlack(RBTreeNode* node);
static void setColor(RBTreeNode* node, Color color);
static RBTreeNode* parentOf(RBTreeNode* node);
static RBTreeNode* grandparentOf(RBTreeNode* node);
static RBTreeNode* siblingOf(RBTreeNode* node);
static RBTreeNode* leftOf(RBTreeNode* node);
static RBTreeNode* rightOf(RBTreeNode* node);
static void rotateLeft(RBTreeNode* n, RBTree* tree);
static void rotateRight(RBTreeNode* n, RBTree* tree);
static void removeFromParent(RBTreeNode* node);
static RBTreeNode* predecessor(RBTreeNode* node);
static void adjustAfterRemoval(RBTreeNode* n, RBTree *tree);
static void setLeft(RBTreeNode* node, RBTreeNode* left);
static void setRight(RBTreeNode* node, RBTreeNode* right);
static Color colorOf(RBTreeNode* node);

RBTreeNode* rbtree_node_constructor(KEY_TYPE key, VALUE_TYPE value, RBTreeNode* parent) {
	RBTreeNode *node = malloc(sizeof(RBTreeNode));
	node->right = NULL;
	node->left = NULL;
	node->parent = parent;
	node->color = Red;
	node->value = value;
	node->key = key;
	return node;
}

RBTree* rbtree_constructor_print( void (*tree_print_function)(void*), int (*tree_compare_function)(void*, void*) ){
	RBTree *tree = malloc(sizeof(RBTree));
	tree->size = 0;
	tree->root = NULL;
	tree->tree_print_function = tree_print_function;
	tree->tree_compare_function = tree_compare_function;
	return tree;
}

RBTree* rbtree_constructor( int (*tree_compare_function)(void*, void*) ){
	RBTree *tree = malloc(sizeof(RBTree));
	tree->size = 0;
	tree->root = NULL;
	tree->tree_print_function = NULL;
	tree->tree_compare_function = tree_compare_function;
	return tree;
}

void rbtree_add(KEY_TYPE key, VALUE_TYPE value, RBTree* tree){
	tree->size++;
	
	if(tree->root == NULL){
		RBTreeNode* newNode = rbtree_node_constructor(key, value, NULL);
		newNode->color = Black; // this needs to be black or else it tries to get sibling of root which is NULL.
		tree->root = newNode;
		return;
	}
	RBTreeNode* itr = tree->root;
	while(itr != NULL){
		if( compare(key, itr->key, tree) > 0){
			if(rightOf(itr) == NULL){
				RBTreeNode* newNode = rbtree_node_constructor(key, value, itr);
				setRight(itr, newNode);
				adjustAfterInsertion(newNode, tree);
				return;
			}
			itr = rightOf(itr);
		}
		else{
			if(leftOf(itr) == NULL){
				RBTreeNode* newNode = rbtree_node_constructor(key, value, itr);
				setLeft(itr, newNode);
				adjustAfterInsertion(newNode, tree);
				return;
			}
			itr = leftOf(itr);
		}
	}
}

void rbtree_put(KEY_TYPE key, VALUE_TYPE value, RBTree* tree){
	tree->size++;
	
	if(tree->root == NULL){
		RBTreeNode* newNode = rbtree_node_constructor(key, value, NULL);
		setColor(newNode, Black);
		tree->root = newNode;
		return;
	}
	RBTreeNode* itr = tree->root;
	while(itr != NULL){
		if( compare(key, itr->key, tree) == 0){
			itr->value = value;
			return;
		}
		else if( compare(key, itr->key, tree) > 0){
			if(rightOf(itr) == NULL){
				RBTreeNode* newNode = rbtree_node_constructor(key, value, itr);
				setRight(itr, newNode);
				adjustAfterInsertion(newNode, tree);
				return;
			}
			itr = rightOf(itr);
		}
		else{
			if(leftOf(itr) == NULL){
				RBTreeNode* newNode = rbtree_node_constructor(key, value, itr);
				setLeft(itr, newNode);
				adjustAfterInsertion(newNode, tree);
				return;
			}
			itr = leftOf(itr);
		}
	}
}

VALUE_TYPE rbtree_search(KEY_TYPE key, RBTree* tree){
	RBTreeNode* itr = tree->root;
	while(itr != NULL){
		if( compare(key, itr->key, tree) == 0 ){
			return itr->value;
		}
		else if( compare(key, itr->key, tree) > 0 ){
			itr = rightOf(itr);
		}
		else{
			itr = leftOf(itr);
		}
	}
	return NULL;
}

RBTreeNode* rbtree_get(KEY_TYPE key, RBTree* tree){
	RBTreeNode* itr = tree->root;
	while(itr != NULL){
		if( compare(key, itr->key, tree) == 0 ){
			return itr;
		}
		else if( compare(key, itr->key, tree) > 0 ){
			itr = rightOf(itr);
		}
		else{
			itr = leftOf(itr);
		}
	}
	return NULL;
}

void rbtree_stack_print(RBTree *tree){
	Stack *stack = stack_constructor();
	stack_push(tree->root, stack);
	while(stackIsEmpty(stack) == 0){
		RBTreeNode* top = stack_pop(stack);
		if(top->right != NULL){
			stack_push(rightOf(top), stack);
		}
		if(top->left != NULL){
			stack_push(leftOf(top), stack);
		}
		printf("| ");
		print(top->key, tree);
	}
	printf("[%d]\n", tree->size);
}

int rbtree_contains(KEY_TYPE key, RBTree* tree){
	return rbtree_search(key, tree) != NULL;
}

static void adjustAfterInsertion(RBTreeNode* n, RBTree* tree) {
    setColor(n, Red);

    if (n != NULL && n != tree->root && isRed(parentOf(n))) {
        if (isRed(siblingOf(parentOf(n)))) {
            setColor(parentOf(n), Black);
            setColor(siblingOf(parentOf(n)), Black);
            setColor(grandparentOf(n), Red);
            adjustAfterInsertion(grandparentOf(n), tree);
        }
        else if (parentOf(n) == leftOf(grandparentOf(n))) {
            if (n == rightOf(parentOf(n))) {
                rotateLeft(n = parentOf(n), tree);
            }
            setColor(parentOf(n), Black);
            setColor(grandparentOf(n), Red);
            rotateRight(grandparentOf(n), tree);
        }
        else if (parentOf(n) == rightOf(grandparentOf(n))) {
            if (n == leftOf(parentOf(n))) {
                rotateRight(n = parentOf(n), tree);
            }
            setColor(parentOf(n), Black);
            setColor(grandparentOf(n), Red);
            rotateLeft(grandparentOf(n), tree);
        }
    }
    setColor(tree->root, Black);
}

void rbtree_remove(KEY_TYPE key, RBTree* tree) {
    RBTreeNode* node = rbtree_get(key, tree);
    if (node == NULL) {
        return;
    } 
    else if (leftOf(node) != NULL && rightOf(node) != NULL) {
        RBTreeNode* pred = predecessor(node);
        node->key = pred->key;
        node->value = pred->value;
        node = pred;
    }
    RBTreeNode* pullUp = leftOf(node) == NULL ? rightOf(node) : leftOf(node);
    if (pullUp != NULL) {
        if (node == tree->root) {
        	tree->root = pullUp;
        } else if (leftOf(parentOf(node)) == node) {
        	setLeft(parentOf(node), pullUp);
        } else {
            setRight(parentOf(node), pullUp);
        }
        if (isBlack(node)) {
            adjustAfterRemoval(pullUp, tree);
        }
    } else if (node == tree->root) {
        tree->root = NULL;
    } else {
        if (isBlack(node)) {
            adjustAfterRemoval(node, tree);
        }
        removeFromParent(node);
    }
    tree->size--;
}

static void adjustAfterRemoval(RBTreeNode* n, RBTree *tree) {
    while (n != tree->root && isBlack(n)) {
        if (n == leftOf(parentOf(n))) {
            RBTreeNode* sibling = rightOf(parentOf(n));
            if (isRed(sibling)) {
                setColor(sibling, Black);
                setColor(parentOf(n), Red);
                rotateLeft(parentOf(n), tree);
                sibling = rightOf(parentOf(n));
            }
            if (isBlack(leftOf(sibling)) && isBlack(rightOf(sibling))) {
                setColor(sibling, Red);
                n = parentOf(n);
            } else {
                if (isBlack(rightOf(sibling))) {
                    setColor(leftOf(sibling), Black);
                    setColor(sibling, Red);
                    rotateRight(sibling, tree);
                    sibling = rightOf(parentOf(n));
                }
                setColor(sibling, colorOf(parentOf(n)));
                setColor(parentOf(n), Black);
                setColor(rightOf(sibling), Black);
                rotateLeft(parentOf(n), tree);
                n = tree->root;
            }
        } else {
            RBTreeNode* sibling = leftOf(parentOf(n));
            if (isRed(sibling)) {
                setColor(sibling, Black);
                setColor(parentOf(n), Red);
                rotateRight(parentOf(n), tree);
                printf("rotate right\n");
                sibling = leftOf(parentOf(n));
            }
            if (isBlack(leftOf(sibling)) && isBlack(rightOf(sibling))) {
                setColor(sibling, Red);
                n = parentOf(n);
            } else {
                if (isBlack(leftOf(sibling))) {
                    setColor(rightOf(sibling), Black);
                    setColor(sibling, Red);
                    rotateLeft(sibling, tree);
                    printf("rotate left\n");
                    sibling = leftOf(parentOf(n));
                }
                setColor(sibling, colorOf(parentOf(n)));
                setColor(parentOf(n), Black);
                setColor(leftOf(sibling), Black);
                rotateRight(parentOf(n), tree);
                n = tree->root;
            }
        }
    }
    setColor(n, Black);
}

static void removeFromParent(RBTreeNode* node){
	RBTreeNode* parent = parentOf(node);
	if(parent == NULL){return;}
	if(leftOf(parent) == node){
		parent->left = NULL;
	}
	else if(rightOf(parent) == node){
		parent->right = NULL;
	}
	node->parent = NULL;
}

static void rotateLeft(RBTreeNode* n, RBTree* tree){
	RBTreeNode* oldRight = rightOf(n);
	setRight(n, leftOf(oldRight));
	if(n == tree->root){
		tree->root = oldRight;
	}
	else if(leftOf(parentOf(n)) == n){
		setLeft(parentOf(n), oldRight);
	}
	else{
		setRight(parentOf(n), oldRight);
	}
	setLeft(oldRight, n);
}

static void rotateRight(RBTreeNode* n, RBTree* tree){
	RBTreeNode* oldLeft = leftOf(n);
	setLeft(n, rightOf(oldLeft));
	if(n == tree->root){
		tree->root = oldLeft;
	}
	else if(leftOf(parentOf(n)) == n){
		setLeft(parentOf(n), oldLeft);
	}
	else{
		setRight(parentOf(n), oldLeft);
	}
	setRight(oldLeft, n);
}

static RBTreeNode* predecessor(RBTreeNode* node){
	RBTreeNode* n = leftOf(node);
	if(n != NULL){
		while(rightOf(n) != NULL){
			n = rightOf(n);
		}
	}
	return n;
}

static int compare(KEY_TYPE v1, KEY_TYPE v2, RBTree *tree){
	return (*(tree->tree_compare_function))(v1, v2);
}

static int print(VALUE_TYPE v, RBTree *tree){
	(*(tree->tree_print_function))(v);
}

static int isRed(RBTreeNode* node){
	if(node == NULL){
		return 0;
	}
	return node->color == Red;
}
static int isBlack(RBTreeNode* node){
	if(node == NULL){
		return 1;
	}
	return node->color == Black;
}
static void setColor(RBTreeNode* node, Color color){
	node->color = color;
}
static RBTreeNode* parentOf(RBTreeNode* node){
	return node->parent;
}
static RBTreeNode* grandparentOf(RBTreeNode* node){
	return parentOf(parentOf(node));
}
static RBTreeNode* siblingOf(RBTreeNode* node){
	RBTreeNode* parent = parentOf(node);
	if(node == leftOf(parent)){
		return rightOf(parent);
	}
	return leftOf(parent);

}
static RBTreeNode* leftOf(RBTreeNode* node){
	return node->left;
}
static RBTreeNode* rightOf(RBTreeNode* node){
	return node->right;
}
static void setLeft(RBTreeNode* node, RBTreeNode* left){
	node->left = left;
	if(left != NULL){
		left->parent = node;
	}
}
static void setRight(RBTreeNode* node, RBTreeNode* right){
	node->right = right;
	if(right != NULL){
		right->parent = node;
	}
}
static Color colorOf(RBTreeNode* node){
	return node->color;
}
