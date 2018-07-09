#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int data;
	struct Node* left;
	struct Node* right;
	struct Node* parent;
	int height;
	int bf;
}Node;

// helper function to create new node
struct Node* getNewNode(int data) {
	// Node* newNode = new Node();
	// use malloc tho
	struct Node* newNode = malloc(sizeof(struct Node)); 
	// would need to cast return type to (Node*) if using C++ 
	newNode->data = data; // deference pointer newNode and point it at data
	newNode->left = newNode->right = newNode->parent = NULL; // no left or right children right now
	newNode->height = newNode->bf = 0;
	// printf("new node created with data %d\n", newNode->data);
	return newNode;
}

// return max of two ints. used for height
int max_h(int a, int b){
	if (a > b) {
		return a;
	}
	else return b;
}

int getHeight(struct Node *x) {
	// need wrapper function to get height to handle null case
	if (x == NULL) {
		return -1;
	} else {
		return x->height;
	}
}

int getData(struct Node *x) {
	// need wrapper function to get data to handle null case
	if (x == NULL) {
		return -999999;
	} else {
		return x->data;
	}
}

int hasParent(struct Node *x) {
	// helper function: check for parent to handle null case
	if (x->parent) {
		printf("x->parent wasn't null for x = %d\n", x->data);
		return 1;
	} else {
		printf("x->parent was null for x = %d\n", x->data);
		return 0;
	}
}

// only heights above inserted/deleted items change
void updateHeightBf(struct Node *x) {
	struct Node *current = x;
	while (current != NULL) {
		int left_h = getHeight(current->left);
		int right_h = getHeight(current->right);
		current->height = max_h(left_h,right_h) + 1;
		current->bf = left_h - right_h;
		current = current->parent;  
	}
}

struct Node* leftRotate(Node *x) {
	printf("calling leftRotate on %d\n", x->data);
	// mirror image of rightRotate
	Node *x_parent = x->parent;
	int is_left; // 1 if x is a left child, 0 if x is a right child
	if (x_parent == NULL) {
		is_left = -1; // x has no parent
	} else {
		if (x_parent->left == x) {
			is_left = 1;
		} else {
			printf("for leftRotate on 3 (%d), this should fire\n", x->data);
			is_left = 0;
		}
	}

	// y is the current right child of x, which will be lifted up
	Node *y = x->right;
	printf("for x = 3 (%d), y should be 4: y->data is: %d\n", x->data, y->data);

	
	// y_left gets adopted and becomes x_right (it may be null)
	Node *y_left = y->left;
	if (y->left != NULL) {
		x->right = y_left; 	
	} else {
		x->right = NULL;
	}

	// x becomes y's left child, x's parent becomes y
	y->left = x;
	x->parent = y; 
	// y gets x's parent
	// x's parent gets y as ___ child
	y->parent = NULL;
	if (is_left != -1) {
		printf("starting the tree\n");
		y->parent = x_parent;	
	}
	
	if (is_left == 1) {
		x_parent->left = y; 
		printf("!!!!!! this line should not print either !!!!!!\n");
	} 
	else if (is_left == 0){
		printf("for x=3 this should show x_parent->right = y: %d->right = %d\n", x_parent->data, y->data);
		x_parent->right = y;
	}
	else {
		// x had no parent, do nothing
		printf("!!!!!! this line should not print !!!!!!\n");
		;
	}


	// update height and bf on x and x's parents
	updateHeightBf(x);
	
	return y;
}

struct Node* rightRotate(Node *x) {
	printf("calling rightRotate on %d\n", x->data);
	// rightRotate subtree rooted at x; return pointer to new root
	Node *x_parent = x->parent;
	int is_left; // 1 if x is a left child, 0 if x is a right child
	if (x_parent == NULL) {
		is_left = -1; // x has no parent
	} else {
		if (x_parent->left == x) {
			is_left = 1;
		} else {
			is_left = 0;
		}
	}

	// y is the current left child of x, which will be lifted up
	Node *y = x->left;

	
	// y_right gets adopted and becomes x_left (it may be null)
	Node *y_right = y->right;
	if (y->right != NULL) {
		x->left = y_right; 	
	} else {
		x->left = NULL;
	}

	// x becomes y's right child, x's parent becomes y
	y->right = x;
	x->parent = y; 
	// y gets x's parent
	// x's parent gets y as ___ child
	y->parent = NULL;
	if (is_left != -1) {
		y->parent = x_parent;	
	}
	else if (is_left == 1) {
		x_parent->left = y; 
	} 
	else if (is_left == 0){
		x_parent->right = y;
	}
	else {
		// x had no parent, do nothing
		;
	}

	// update height and bf on x and x's parents
	updateHeightBf(x);
	
	return y;
}

struct Node* balance(Node *x) {
	struct Node* new_x = x;
	printf("running balance() on x=%d\n", x->data);
	// we've defined balance as left.height - right.height
	// balance the subtree rooted at x, assuming x->left and x->right are already AVL
	// no need to recurse up

	// A) right imbalance
	if (x->bf < -1) {
		printf("right imbalance detected\n");
	// case 1: right imbalance, right heavy or balanced right subtree
		if ((x->right->bf == -1)||(x->right->bf == 0)) {
			// simple left rotation on x
			new_x = leftRotate(x);
		}
		// case 2: right imbalance, left heavy right subtree
		else {
			// first right-rotate on x->right
			Node* x_right = x->right;
			rightRotate(x_right);
			// then left-rotate on x
			new_x = leftRotate(x);
		}
	}

	// B) left imbalance
	if (x->bf > 1) {
		printf("left imbalance detected\n");
		// case 1: left imbalance, x.left is left-heavy or balanced
		if ((x->left->bf == 1) || (x->left->bf == 0)) {
			// simple right rotation on x
			new_x = rightRotate(x);
		} 
		// case 2: left imbalance, x.left is right-heavy
		else {
			// first left-rotate on x->left
			Node* x_left = x->left;
			leftRotate(x_left);
			// then right-rotate on x
			new_x = rightRotate(x);
		}
	}
	
	return new_x;
}

void insert(struct Node** root, struct Node** parent, int data, struct Node **rootTracker) {
	// make an insertion into the subtree rooted at root
	// set rootTracker to the new topRoot
	struct Node *newroot = getNewNode(0);

	printf("********************* attempting to insert %d\n", data); 
	// needs to take pointer to pointer to root
	// otherwise value stays local; and returning is wasteful

	struct Node *null_ptr = NULL;  
	
	// case 1: tree is empty
	if((*root) == NULL) {
		(*root) = getNewNode(data);
		printf("********************* inserted %d\n", (*root)->data);
		
		if (parent != NULL) {

			// set parent of inserted item
			// printf("trying to set parent\n");
			(*root)->parent = *parent;
			// printf("parent set to %d\n", (*root)->parent->data);

			// set parent's left or right child to inserted item
			if (data <= (*parent)->data) {
				(*parent)->left = *root;
			}
			else {
				(*parent)->right = *root;
			}
			updateHeightBf(*parent);

			// only balance of inserted item and its parents can be affected
			// so check up the tree
			// also be sure to update root after balancing
			struct Node *current = *root;
			//printf("runs fine to here\n");
			newroot = balance(current);
			while (current->parent) {
				printf("fine\n");
				current = current->parent;
				newroot = balance(current); 
				// printf("1. newroot is now: %d\n", newroot->data);
				(*rootTracker) = newroot;
				// printf("root->data is now: %d\n", (*root)->data);
			}
		}
	}
	else if(data < (*root)->data) {
		// less than root; go to left side, recursively call insert on left child
		if ((*root)->left) {
			insert(&((*root)->left), root, data, rootTracker);
		}
		else {
			insert(&null_ptr, root, data, rootTracker);
		}
	}
	else if(data >= (*root)->data) {
		// greater than root's data; recursively call right side
		// also allow dupes and place to the right
		// notice we need to deref once, get right, then get address
		if ((*root)->right) {
			insert(&((*root)->right), root, data, rootTracker);
		}
		else {
			insert(&null_ptr, root, data, rootTracker);
		}
	}
	// printf("2. rootTracker is now: %d\n", (*rootTracker)->data);
	// return newroot->data;
}

struct Node* search(struct Node** root, int data) {
	if (*root == NULL) {
		// tree is empty
		return NULL;
	}
	else if(data == (*root)->data) {
		return *root;	
	}
	// now start recursively looking down the left and right subtrees
	else if (data < (*root)->data) {
		// value is smaller; recursively look to the left
		return search(&((*root)->left), data);
	}
	else { // (data > (*root)->data)
		// look to right
		return search(&((*root)->right), data);
	}
}

int min(struct Node** x) { // case where empty? x is NULL?
	if (*x == NULL) {
		printf("ERROR: tree is empty"); //should throw exception
	}
	while ((*x)->left != NULL) {
		x = &((*x)->left);	
	}
	return (*x)->data;
}

int max(struct Node** x) {
	if (*x == NULL) {
		printf("ERROR: tree is empty"); //should throw exception
	}
	while ((*x)->right != NULL) {
		x = &((*x)->right);	
	}
	return (*x)->data;
}

struct Node* delete(struct Node **root, int data, struct Node **rootTracker) { 
// will run this recursively in order to "find" the node
// first if-else tree takes care of setting parent references to NULL

	if (*root == NULL) return *root; // tree is empty, return null
	else if (data < (*root)->data) {
		// look left
		(*root)->left = delete(&((*root)->left), data, rootTracker);
	}
	else if (data > (*root)->data) {
		// look right
		(*root)->right = delete(&((*root)->right), data, rootTracker);
	}
	else {
		// equal! found ya. this tree takes care of freeing memory
		// update height on root's parent and each of its parents
		struct Node *newroot = getNewNode(0);

		(*root)->height -= 1;
		updateHeightBf((*root)->parent);

		// only balance of inserted item and its parents can be affected
		// so check up the tree
		// also be sure to update root after balancing
		struct Node *current = *root;
		newroot = balance(current);
		while (current->parent) {
			current = current->parent;
			newroot = balance(current); 
			(*rootTracker) = newroot;
		}


		// Case 1: no children
		if ((*root)->left == NULL && (*root)->right == NULL) { 
			free(*root); // free the memory being pointed at by root
			*root = NULL; // set its value to null
		}
		// case 2.1: has right child leaf
		else if ((*root)->left == NULL) {
			// replace node equal to child
			// store address of root in temp var
			struct Node *temp = *root;
			*root = (*root)->right; // *root now points to right child's address
			free(temp); // free memory pointed at by temp (old root address)
		}
		// case 2.2: has left child leaf
		else if ((*root)->right == NULL) {
			// replace node equal to child
			// store address of root in temp var
			struct Node *temp = *root;
			*root = (*root)->left; // *root now points to left child's address
			free(temp); // free memory pointed at by temp (old root address)
		}
		// case 3: has 2 children
		else {
			// look for successor: minimum of right subtree
			struct Node *temp = (*root)->right;
			// get min of right tree BY GOING LEFT
			while (temp->left != NULL) {
				temp = temp->left;	
			}
			(*root)->data = temp->data;
			(*root)->right = delete(&((*root)->right), temp->data, rootTracker);
		}
	}
	return *root;
}

void inOrderWalk(struct Node** x) {
	if((*x) != NULL ) {

	// Node** x is pointer to root
	struct Node *left = (*x)->left;
	struct Node *right = (*x)->right;

	printf("---------- calling IOWL\n");
	inOrderWalk(&left);
	printf("---------- IOWL complete\n");
	printf("IOW: %d\n", (*x)->data);
	printf("---------- calling IOWR\n");
	inOrderWalk(&right);
	printf("---------- IOWR complete\n");
	}
}

void inOrderWalkCount(struct Node** x,  int *counter) {
	if((*x) != NULL ) {

	// Node** x is pointer to root
	struct Node *left = (*x)->left;
	struct Node *right = (*x)->right;

	inOrderWalkCount(&left, counter);
	printf("IOWC: %d\n", (*x)->data);
	*counter += 1;
	inOrderWalkCount(&right, counter);
	}
}

void preOrderWalk(struct Node** x) {
	if(*x != NULL) {
		printf("%d\n", (*x)->data);
		preOrderWalk(&((*x)->left));
		preOrderWalk(&((*x)->right));
	}
}

void postOrderWalk(struct Node** x) {
	if(*x != NULL) {
		printf("%d\n", (*x)->data);
		preOrderWalk(&((*x)->left));
		preOrderWalk(&((*x)->right));
	}
}

// void insert(struct Node** root, struct Node** parent, int data, struct Node **rootTracker)
struct Node* insertWrapper(struct Node** root, int data) {
	struct Node* newRoot;
	insert(root, NULL, data, &newRoot);
	return newRoot;
}

struct Node* deleteWrapper(struct Node** root, int data) {
	struct Node* newRoot;
	delete(root, data, &newRoot);
	return newRoot;
}

int extractSecondLargest(struct Node** x) { // extract and delete second largest
	// LOL oops this was for wrong q....good learning experience tho
	if (*x == NULL) {
		printf("ERROR: tree is empty"); //should throw exception
	}
	struct Node *one_up = NULL;

	while ((*x)->right != NULL) {
		one_up = *x;
		x = &((*x)->right);	
	}
	int secondLargest = (one_up)->data;
	deleteWrapper(x, secondLargest);
	return secondLargest;
}	

int main() {
	// need to store a pointer to root which refers to the entire tree
	// start with rootPtr = NULL for empty tree
	struct Node* root = malloc(sizeof(struct Node*));
	root = NULL;

	// insert(&root, NULL, 15);
	// insert(&root, NULL, 6);
	// insert(&root, NULL, 18);
	// insert(&root, NULL, 3);
	// insert(&root, NULL, 7);
	// insert(&root, NULL, 17);
	// insert(&root, NULL, 20);
	// insert(&root, NULL, 2);
	// insert(&root, NULL, 4);
	// insert(&root, NULL, 13);

	// root = delete(&root, 15);
	// root = delete(&root, 20);




	insert(&root, NULL, 1, NULL); // janky
	printf("root after inserting 1 is: %d\n", root->data);
	inOrderWalk(&root);
	printf("-------\n");
	
	root = insertWrapper(&root, 2);
	printf("root after inserting 2 is: %d\n", root->data);
	inOrderWalk(&root);
	printf("-------\n");

	root = insertWrapper(&root, 3);
	printf("root after inserting 3 is: %d\n", root->data);
	inOrderWalk(&root);
	printf("-------\n");

	root = insertWrapper(&root, 4);
	printf("root after inserting 4 is: %d\n", root->data);
	inOrderWalk(&root);
	printf("-------\n");

	root = insertWrapper(&root, 5);
	printf("root after inserting 5 is: %d\n", root->data);
	inOrderWalk(&root);
	printf("-------\n");

	root = insertWrapper(&root, 6);
	printf("root after inserting 6 is: %d\n", root->data);
	inOrderWalk(&root);
	printf("-------\n");

	root = deleteWrapper(&root, 2);
	printf("root after deleting 2 is: %d\n", root->data);
	inOrderWalk(&root);
	printf("-------\n");

	root = NULL;
	free(root);


	return 0;
};