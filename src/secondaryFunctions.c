#include "../inc/rcm.h"
#include <omp.h>

// This is a helper function for qsort.
// It is used in order to perform qsort to an array of structs
int compare (const void * a, const void * b)
{
  const Node *nodeA = *((Node **)a);
  const Node *nodeB = *((Node **)b);
  return nodeA->degree - nodeB->degree;
}

// This function pushes a node's index to R Array 
// and updates Array's index
void pushArray(Array * R, Node * nd)
{
    if(R->idx == R->size)
    {
        printf("Error!\nR is full, can't push node to Array\n");
        exit(-1);
    }
    R->nodeIdx[R->idx] = nd->idx;
    R->idx++;
    nd->inR=1;
}

// This function pushes node "nd" to Graph G.
// Graph's tail index is updated. Also nd->inQ is 
// set to 1 to indicate that node "nd" has been placed in Q
void pushGraph(Graph * G, Node * nd)
{
    if(G->tail == G->size)
    {
        printf("Error!\nQueue is full, can't push node to Q\n");
        exit(-1);
    }
    G->nodes[G->tail] = nd;
    G->tail++;
    nd->inQ=1;   
    
}

// This function extracts a node from Graph G.
// Graph's head index is updated to point at the 
// node that will be extracted at the next iteration
Node * popGraph(Graph * G)
{
    if (G->head == -1)
    {
        printf("Error at pop, Queue is empty!\n");
        return NULL;
    }
    Node * returnNode = G->nodes[G->head];
    G->head++;
    return returnNode;
}

// This function generates the final graph based on the initial graph G
// and the permutations R.
Graph * finalizeGraph(Graph * G, Array * R)
{
    Graph * F = (Graph *)malloc(sizeof(Graph*));
    F->size = G->size;
    F->nodes = (Node **)malloc(F->size*sizeof(Node));
    for(int i=0; i<F->size; i++)
    {
        F->nodes[i] = (Node*)malloc(sizeof(Node));
        F->nodes[i]->idx = i;
        F->nodes[i]->degree = G->nodes[R->nodeIdx[i]]->degree;
    }
    int index;
    int j;
    // this omp optimizations do not contribute to the core algorithm
    // performance rather than the program's performance overall
    #pragma omp parallel for num_threads(4) private(j)
    for(int i=0; i<F->size; i++)
    {
        F->nodes[i]->neighbours = (Node**)malloc( F->nodes[i]->degree*sizeof(Node));
        
        int k;
        #pragma omp parallel for num_threads(4) private(k, index)
        for( j=0; j< F->nodes[i]->degree; j++)
        {
            for(k=0; k<R->size; k++)
            {
                if(G->nodes[R->nodeIdx[i]]->neighbours[j]->idx == R->nodeIdx[k])
                {
                    index = k;
                }
            }
            
            F->nodes[i]->neighbours[j] =  F->nodes[index];
        }
    }
    return F;
}


// This function creates the Final Sparse Matrix from the final Graph
void graphToSparseMatrix(Graph * finalG, int size, int *arr)
{   
    int j;
    #pragma omp parallel for num_threads(8) private(j) collapse(2)
    for(int i=0; i<size; i++)
    {
        for(j=0; j<size; j++)
        {
            if(i==j)
            {
                arr[size*i+j] = 1;
            }
            else
            {
                arr[size*i+j] = 0;
            }
            
        }
    }
        
    for(int i=0; i<finalG->size; i++)
    {
        for(j=0; j<finalG->nodes[i]->degree; j++)
        {
            arr[size*(finalG->nodes[i]->idx) + finalG->nodes[i]->neighbours[j]->idx] = 1;
        }
    }
}


// This function saves a col-major int array to a .csv file in /sparseMatrices directory
void saveCsv (int *arr, int size, int input)
{
    char path[60];
    if(input)
    {
        strcpy(path, "../sparseMatrices/input");
    }
    else
    {
       strcpy(path,"../sparseMatrices/output");
    }
    
    char name[8];
    char type[8] = ".csv";
    sprintf(name, "%d", size);
    strcat(path, name);
    strcat(path, type);
    FILE *f = fopen(path, "w");
    if (f == NULL) exit(1);
    for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
        {
            fprintf(f,"%d,",arr[size*i+j]);
        }
        fprintf(f,"\n");
    }
}

// This function saves a col-major int array to a .txt file in /sparseMatrices directory
void saveTxt(int *arr, int size)
{
    char path[50] = "../sparseMatrices/input";
    char name[8];
    char type[8] = ".txt";
    sprintf(name, "%d", size);
    strcat(path, name);
    strcat(path, type);
    FILE *file = fopen(path, "w");
    for(int i=0; i<size*size; i++)
    {
        if (fprintf(file,"%d", arr[i]) != 1)
        {
            printf("Error failed to create the txt file\n");
        }
    }
    fclose(file);
}

// This function reads a sparse Matrix from a .txt file and stores it in a col-major array 
int *readTxt(char * filename, int size)
{
    int *arr = (int *)malloc(size*size*sizeof(int));
    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file\n");
        exit(1);
    }
    for (int i = 0; i < size*size; i++)
    {
        fscanf(fp, "%1d", &arr[i]);
    }
    fclose(fp);
    return arr;
}

// Swap positions of two elements in R->nodeIdx array
void swap(int * a, int * b) 
{ 
    int t = *a; 
    *a = *b; 
    *b = t; 
}

// This function generates the initial graph produced
// from the initial Sparse Matrix
Graph * initializeGraph(int *arr, int size)
{
    Graph * graph = (Graph *)malloc(sizeof(Graph));
    graph->size = size; 
    graph->nodes = (Node **)malloc(graph->size*sizeof(Node));
    // Initialize nodes
    for(int i=0; i<graph->size; i++)
    {
        graph->nodes[i] = (Node *)malloc(sizeof(Node));
        graph->nodes[i]->neighbours = (Node**)malloc(size*sizeof(Node));
        graph->nodes[i]->degree = 0;
        graph->nodes[i]->idx = i;
        graph->nodes[i]->inQ = 0;
        graph->nodes[i]->inR = 0;
    } 
    graph->tail = size-1;
    graph->head = 0;
    // Construct each node's degree and neighbours based on the initial sparse matrix
    for(int i=0; i<graph->size; i++)
    {
        int counter = 0;
        for(int j=0; j<graph->size; j++)
        {
            if (i==j)
                continue;
            if(arr[size * i + j])
            {
                // Traverse the original matrix and calculate degree & 
                // neighbours for each graph node
                graph->nodes[i]->degree++;
                //graph->nodes[i]->neighbours[counter] = (Node*)malloc(sizeof(Node));
                graph->nodes[i]->neighbours[counter] = graph->nodes[j];
                counter++;
            }
                 
        }
    }
    return graph;
}

// This function generates the initial graph produced
// from the initial Sparse Matrix
Graph * parallelInitGraph(int *arr, int size)
{
    Graph * graph = (Graph *)malloc(sizeof(Graph));
    graph->size = size; 
    graph->nodes = (Node **)malloc(graph->size*sizeof(Node));
    // Initialize nodes
    for(int i=0; i<graph->size; i++)
    {
        graph->nodes[i] = (Node *)malloc(sizeof(Node));
        graph->nodes[i]->neighbours = (Node**)malloc(size*sizeof(Node));
        graph->nodes[i]->degree = 0;
        graph->nodes[i]->idx = i;
        graph->nodes[i]->inQ = 0;
        graph->nodes[i]->inR = 0;
    } 
    graph->tail = size-1;
    graph->head = 0;

    // get as much thread processing power as twice 
    // the device's physical cores
    int numthreads = 2 * omp_get_max_threads();
    printf("Number of threads = %d\n", numthreads);
    int j, counter;
    // Construct each node's degree and neighbours based on the initial sparse matrix
    #pragma omp parallel for num_threads(numthreads) private(j,counter) if(graph->size > 2000)
    for(int i=0; i<graph->size; i++)
    {  
        counter = 0;
        for(j = 0 ; j < graph->size ; j++)
        {
            if (i==j)
                continue;
            if(arr[size * i + j])
            {
                // Traverse the original matrix and calculate degree & 
                // neighbours for each graph node
                graph->nodes[i]->degree++;
                graph->nodes[i]->neighbours[counter] = graph->nodes[j];
                counter++;
            }
        }
    }
    return graph;
}

// This function sorts the nodes of a graph based on 
// their degrees. Also, each node's neighbours are sorted
// according to their degrees.
void sortGraph(Graph * G)
{
    for(int i=0; i<G->size; i++)
    {
        // Sort neighbours of each node, based on neighbours' degrees  
        qsort(G->nodes[i]->neighbours, G->nodes[i]->degree, sizeof(G->nodes[i]->neighbours), compare);
    }
    // Sort nodes based on nodes' degree  
    qsort(G->nodes, G->size, sizeof(G->nodes), compare);
}

// Calculate the bandwidth of a given sparse matrix in the form
// of a col-major array
int bandwith(int *Arr, int size)
{
	int high = 0;
	int low = 0;
	int temp, band;

	for (int i = 0; i < size; i++)
	{
		temp = 0;
		for (int j = size - 1; j > i; j--)
			if (Arr[size * i + j] != 0)
			{
				temp = j - i;
				break;
			}
			else
				continue;

		if (temp > high)
			high = temp;

		temp = 0;
		for (int j = 0; j < i; j++)
			if (Arr[size * i + j] != 0)
			{
				temp = i - j;
				break;
			}
			else
				continue;

		if (temp > low)
			low = temp;
	}

	band = low+high + 1;

	return band;
}

// Crate a random square sparse matrix given the size dimension and density
int * generateSparseMatrix(int size, double density)
{
    int *arr = (int *)malloc(size*size*sizeof(int));
    srand(time(0)); 
    for(int i=0; i<size; i++)
    {
        for(int j=i; j<size; j++)
        {
            if(i == j)
                arr[size*i + j] = 1;
            else
            {
                double bin = (double)rand() / RAND_MAX;
                if( bin <= 0.01*density ) 
                    arr[size*i + j] = 1;
                else
                    arr[size*i + j] = 0;
            }
        }

        for(int j=0; j<i; j++)
            arr[size*i + j] = arr[size*j + i];
    }
    return arr;
}