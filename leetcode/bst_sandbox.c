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

struct Node* searchNode(struct Node **root, int data) { // returns pointer to node

	if (*root == NULL) {
		// tree is empty
		return 0; // NULL is macro for address 0 anyway
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

struct Node* Delete(struct Node **root, int data) { 
// delete data from tree rooted at root
// case 1: is at leaf
	if (searchNode(root, data)->left == NULL 
		&& searchNode(root, data)->right == NULL) {
		
	}

// case 2: has one child
// case 3: has 3 children

// need to free memory from heap once deleted

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
	delete(one_up);
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
	
	return 0;
}