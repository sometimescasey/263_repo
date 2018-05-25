#include <stdio.h>
#include <stdlib.h>

struct Node {
	int data;
	struct Node* left;
	struct Node* right;
};

// helper function to create new node
struct Node* getNewNode(int data) {
	// Node* newNode = new Node();
	// use malloc tho
	struct Node* newNode = malloc(sizeof(struct Node)); 
	// would need to cast return type to (Node*) if using C++ 
	newNode->data = data; // deference pointer newNode and point it at data
	newNode->left = newNode->right = NULL; // no left or right children right now
	return newNode;
}

void insert(struct Node** root, int data) { // needs to take pointer to pointer to root
	// otherwise value stays local; and returning is wasteful
	// case 1: tree is empty
	if(*root == NULL) {
		*root = getNewNode(data);
	}
	else if(data < (*root)->data) {
		// less than root; go to left side, recursively call insert on left child
		insert(&((*root)->left), data);
	}
	else if(data >= (*root)->data) {
		// greater than root's data; recursively call right side
		// also allow dupes and place to the right
		// notice we need to deref once, get right, then get address
		insert(&((*root)->right), data); 
	}
}

int search(struct Node** root, int data) {
	if (*root == NULL) {
		// tree is empty
		return 0;
	}
	else if(data == (*root)->data) {
		return 1;	
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

struct Node* delete(struct Node **root, int data) { 
// will run this recursively in order to "find" the node
// first if-else tree takes care of setting parent references to NULL
	if (*root == NULL) return *root; // tree is empty, return null
	else if (data < (*root)->data) {
		// look left
		(*root)->left = delete(&((*root)->left), data);
	}
	else if (data > (*root)->data) {
		// look right
		(*root)->right = delete(&((*root)->right), data);
	}
	else {
		// equal! found ya. this tree takes care of freeing memory
		// Case 1: no children
		if ((*root)->left == NULL && (*root)->right == NULL) {
			free(*root); // free the memory being pointed at by root
			*root = NULL; // set its value to null
		}
		// case 2.1: has right child
		else if ((*root)->left == NULL) {
			// replace node equal to child
			// store address of root in temp var
			struct Node *temp = *root;
			*root = (*root)->right; // *root now points to right child's address
			free(temp); // free memory pointed at by temp (old root address)
		}
		// case 2.2: has left child
		else if ((*root)->right == NULL) {
			// replace node equal to child
			// store address of root in temp var
			struct Node *temp = *root;
			*root = (*root)->left; // *root now points to left child's address
			free(temp); // free memory pointed at by temp (old root address)
		}
		// case 3: has 2 children
		else {
			// store address of root in temp var
			struct Node *temp = *root;
			// get min of right tree BY GOING LEFT
			while (temp->left != NULL) {
				temp = temp->left;	
			}
			(*root)->data = temp->data;
			(*root)->right = delete(&((*root)->right), temp->data);
		}
	}
	return *root;
}

int extractSecondLargest(struct Node** x) { // extract and delete second largest
	if (*x == NULL) {
		printf("ERROR: tree is empty"); //should throw exception
	}
	struct Node *one_up = NULL;

	while ((*x)->right != NULL) {
		one_up = *x;
		x = &((*x)->right);	
	}
	int secondLargest = (one_up)->data;
	delete(x, secondLargest);
	return secondLargest;
}

	

int main() {
	// need to store a pointer to root which refers to the entire tree
	// start with rootPtr = NULL for empty tree
	struct Node* root = NULL;
	insert(&root, 15);
	insert(&root, 10);
	insert(&root, 21);
	insert(&root, 25);
	insert(&root, 8);
	insert(&root, 12);
	insert(&root, -1);

	int number;
	printf("Enter a number:\n");
	scanf("%d", &number);
	if (search(&root, number)) {
		printf("Found %d!\n", number);
	}
	else {
		printf("Did not find %d :(\n", number);
	}

	printf("Max is: %d\n", max(&root));
	printf("Min is: %d\n", min(&root));

	// extractSecondLargest() unit test
	// return min, max
	// calc second largest, remove it
	// return min, max, should not change
	printf("second largest is: %d\n", extractSecondLargest(&root));

	delete(&root, 21);
	printf("21 was deleted\n");
	delete(&root, -1);
	printf("-1 was deleted\n");
	printf("Max is: %d\n", max(&root));
	printf("Min is: %d\n", min(&root));
	printf("second largest is: %d\n", extractSecondLargest(&root));

	
	return 0;
}