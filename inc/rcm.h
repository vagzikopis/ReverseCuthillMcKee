#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

// Node struct used to represent Nodes of a Graph
typedef struct Node
{
    // Array to hold neighbour nodes of every node
    struct Node ** neighbours;
    // Node's degree
    int degree;
    // Node's index
    int idx;
    // Boolean attribute.
    // 1-(True) if the node has been placed in Q, 0-(False) otherwise
    int inQ;
    // Boolean attribute.
    // 1-(True) if the node has been placed in R, 0-(False) otherwise
    int inR;
}Node;

// Graph struct used to represent the Sparse Matrix as a graph
typedef struct Graph
{
    // Array to hold graph's nodes
    Node ** nodes;
    // Graph size
    int size;
    // Index pointing at the node that will be extracted from the graph.
    // Head index is used at the composition of Q
    int head;
    // Index pointing at the last inserted node of the graph.
    // Head index is used at the composition of Q
    int tail;
}Graph;

// Array struct used for R
typedef struct Array
{
    // Array holding nodes' indexes. This is the final permutation array
    int * nodeIdx;
    // Index pointing at the last inserted node of the Array R.
    int idx;
    // Array size
    int size;
}Array;


// This function generates the initial graph produced
// from the initial Sparse Matrix
Graph * initializeGraph(int *arr, int size);

// This function generates the initial graph produced
// from the initial Sparse Matrix in parallel
Graph * parallelInitGraph(int *arr, int size);

// This function generates the permutation Array R.
// R has the form of an Array struct.
Array * rcm(int * arr, int size);

// This function generates the final graph based on the initial graph G
// and the permutations R.
Graph * finalizeGraph(Graph * G, Array * R);

// This function creates the Final Sparse Matrix from the final Graph
void graphToSparseMatrix(Graph * finalG, int size, int *arr);

// This function saves a col-major int array to a .csv file int matrices directory
void saveCsv (int *arr, int size, int input);

// This function sorts the nodes of a graph based on 
// their degrees. Also, each node's neighbours are sorted
// according to their degrees.
void sortGraph(Graph * G);

// Swap positions of two elements in R->nodeIdx array
void swap(int * a, int * b);

// This function extracts a node from Graph G.
// Graph's head index is updated to point at the 
// node that will be extracted at the next iteration
Node * popGraph(Graph * G);

// This function pushes node "nd" to Graph G.
// Graph's tail index is updated. Also nd->inQ is 
// set to 1 to indicate that node "nd" has been placed in Q
void pushGraph(Graph * G, Node * nd);

// This function pushes a node's index to R Array 
// and updates Array's index
void pushArray(Array * R, Node * nd);

// This is a helper function for qsort.
// It is used in order to perform qsort to an array of structs
int compare (const void * a, const void * b);

// This function saves a col-major int array to a .txt file in sparseMatrices directory
void saveTxt(int *arr, int size);

// This function reads a sparse Matrix from a .txt file and stores it in a col-major array 
int *readTxt(char * filename, int size);

// Calculates the bandwith of a sparse Matrix
int bandwith(int *Arr, int size);

// Creates a size * size random sparse Matrix
int * generateSparseMatrix(int size, double density);
