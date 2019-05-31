#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include "avl.h"
void preorder(struct AVLnode *node, TYPE *min_cost, TYPE *path, int *m, TYPE *candidate_path, int *n, TYPE sumDiff, TYPE parent_value);
TYPE absoluteDiff(TYPE a, TYPE b);
int FindMinPath(struct AVLTree *tree, TYPE *path);
void printBreadthFirstTree(struct AVLTree *tree);


/* --------------------
Finds the minimum-cost path in an AVL tree
   Input arguments:
        tree = pointer to the tree,
        path = pointer to array that stores values of nodes along the min-cost path,
   Output: return the min-cost path length

   pre: assume that
       path is already allocated sufficient memory space
       tree exists and is not NULL
*/
int FindMinPath(struct AVLTree *tree, TYPE *path)
{
        int path_len = 0; /* the initial length of the min-cost path */
        struct AVLnode * current = tree->root;
        TYPE min_cost = (TYPE) 10^6 * tree->cnt; /* initial high value for minimum */
        int c_path_len = 0; /* length of a candidate path */
        TYPE candidate_path[100]; /* candidate path is a static array */

        path[path_len] = tree->root->val; /* min-cost path must contain the root */
        path_len++;


        /* Write this part of the function */
        if (tree->cnt > 1){

               /* Traverse the tree and find the min-cost path */

               /* FIX ME */
               preorder(current, &min_cost, path, &path_len, candidate_path, &c_path_len, 0, current->val);

        }
	return path_len;
}

/* ----------
Finds absolute difference between two input numbers
*/
TYPE absoluteDiff(TYPE a, TYPE b){

   if (a<0)
     return (TYPE) 0;
   else
     return (TYPE) abs(a-b);
}


/*-------------------------
Recursively traverses the AVL tree and finds its min cost path
Input argument:
     node = pointer to the current node visited by recursion,
     min_cost = the latest minimum cost,
     path = array of values of nodes along the latest best path,
     path_len = number of elements in path array,
     candidate_path = currently considered path array
     c_path_len = number of elements in the candidate path array

pre: assume that all input arguments are well initialized and have enough memory space

post:
     path points to the array of values in the min-cost path of the AVL tree
     path_len is the number of elements (i.e., nodes) in the min-cost path of the AVL tree
*/
void preorder(struct AVLnode *node, TYPE *min_cost, TYPE *path, int *path_len,
               TYPE *candidate_path, int *c_path_len, TYPE sumDiff, TYPE parent_value)
{

    if(node != NULL) {

        /* put the current node in a candidate path */
        candidate_path[*c_path_len] = node->val;
        (*c_path_len)++;

        /* cumulative cost of the candidate path */
        sumDiff = sumDiff + absoluteDiff(parent_value, node->val);

        /* FIX ME */
        /* Write the recursion case(s) and the stopping-recursion case(s) */

        /* process value */
        printf("%d ", node->val);

        preorder(node->left, min_cost, path, path_len, candidate_path, c_path_len, sumDiff, parent_value);
        preorder(node->right, min_cost, path, path_len, candidate_path, c_path_len, sumDiff, parent_value);
    }


}

int AVLQueueAddBack(struct AVLnode *item, struct AVLnode ** queue, int *back, int *size, int capacity) {
    if (*size >= capacity) return 0;

    *back = (*back + 1) % capacity;
    queue[*back] = item;
    *size += 1;

    return 1;
}

struct AVLnode *AVLQueueRemoveFront(struct AVLnode ** queue, int *front, int *size, int capacity) {
    struct AVLnode *item;

    if (*size == 0) return NULL;

    item = queue[*front];
    *front = (*front + 1) % capacity;
    *size -= 1;

    return item;
}


/* -----------------------
Printing the contents of an AVL tree in breadth-first fashion
  param: pointer to a tree
  pre: assume that tree was initialized well before calling this function
*/
void printBreadthFirstTree(struct AVLTree *tree){
    int QUEUE_CAPACITY = 100;
    int i;
   struct AVLnode **queue;  /* print using a queue, where queue is implemented as a static array */
   struct AVLnode *current = tree->root;
   int start = 0; /* start index of queue indicating the first element to be processed */
   int end = QUEUE_CAPACITY - 1;  /* end index of queue indicating the latest element added to the queue */
   int size = 0;

   /* allocate memory to queque */
   queue = (struct AVLnode **) malloc(QUEUE_CAPACITY*sizeof(struct AVLnode));
   assert(queue);


    /* FIX ME */

    /* Make sure queue is not empty */
    if(current == NULL) return;

    /* Add first item to queue back */
    AVLQueueAddBack(current, queue, &end, &size, QUEUE_CAPACITY);
    i = 0;
    while(size < QUEUE_CAPACITY) {
        /* Remove as current and print */
        current = AVLQueueRemoveFront(queue, &start, &size, QUEUE_CAPACITY);

        if(current == NULL) break;

        printf("%d  ", current->val);

        if(current->left != NULL) {
            /* Add left */
            AVLQueueAddBack(current->left, queue, &end, &size, QUEUE_CAPACITY);
        }

        if(current->right != NULL) {
            /* Add right */
            AVLQueueAddBack(current->right, queue, &end, &size, QUEUE_CAPACITY);
        }
        i++;

    }

    free(queue);
}


/* -----------------------
The main function
  param: argv = pointer to the name (and path) of a file that the program reads for adding elements to the AVL tree
*/
int main(int argc, char** argv) {

	FILE *file;
	int len, i;
	TYPE num; /* value to add to the tree from a file */
	struct timeval stop, start; /* variables for measuring execution time */
	int pathArray[50];  /* static array with values along the min-cost path of the AVL tree */
	struct AVLTree *tree;

	tree = newAVLTree(); /*initialize and return an empty tree */

	file = fopen(argv[1], "r"); 	/* filename is passed in argv[1] */
	assert(file != 0);

	/* Read input file and add numbers to the AVL tree */
	while((fscanf(file, "%i", &num)) != EOF){
		addAVLTree(tree, num);
	}
	/* Close the file  */
	fclose(file);

	printf("\nPrinting the tree breadth-first : \n");
	printBreadthFirstTree(tree);

	gettimeofday(&start, NULL);

	/* Find the minimum-cost path in the AVL tree*/
	len = FindMinPath(tree, pathArray);

	gettimeofday(&stop, NULL);

	/* Print out all numbers on the minimum-cost path */
	printf("\nThe minimum-cost path is: \n");
	for(i = 0; i < len; i++)
		printf("%d ", pathArray[i]);
	printf("\n");

	printf("\nYour execution time to find the mincost path is %f microseconds\n", (double) (stop.tv_usec - start.tv_usec));

        /* Free memory allocated to the tree */
	deleteAVLTree(tree);

	return 0;
}
