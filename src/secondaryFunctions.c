#include "../inc/rcm.h"


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
Graph * finalGraph(Graph * G, Array * R)
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
    for(int i=0; i<F->size; i++)
    {
        F->nodes[i]->neighbours = (Node**)malloc( F->nodes[i]->degree*sizeof(Node));
        for(int j=0; j< F->nodes[i]->degree; j++)
        {
            for(int k=0; k<R->size; k++)
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
     for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
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
        for(int j=0; j<finalG->nodes[i]->degree; j++)
        {
            arr[size*(finalG->nodes[i]->idx) + finalG->nodes[i]->neighbours[j]->idx] = 1;
        }
    }
}

// This function saves a col-major int array to a .csv file int matrices directory
void saveCsv (int *arr, char * filename, int size)
{
    FILE *f = fopen(filename, "w");
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
                graph->nodes[i]->degree++;
                graph->nodes[i]->neighbours[counter] = (Node*)malloc(sizeof(Node));
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
        qsort (G->nodes[i]->neighbours, G->nodes[i]->degree, sizeof(G->nodes[i]->neighbours), compare);
    }
    // Sort nodes based on nodes' degree  
    qsort (G->nodes, G->size, sizeof(G->nodes), compare);
}

