#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/rcm.h"
#include <sys/time.h>

// This function generates the permutation Array R.
// R has the form of an Array struct.
Array * rcm(int * arr, int size)
{
    // Generate the initial graph
    Graph * graph = initializeGraph(arr, size);
    sortGraph(graph);
    struct timeval start, end;
    Array * R = (Array *)malloc(sizeof(Array));
    R->size = graph->size;
    R->nodeIdx = (int *)malloc(R->size*sizeof(int));
    R->idx = 0;

    Graph * Q = (Graph *)malloc(sizeof(Graph));
    Q->size = graph->size;
    Q->nodes = (Node **)malloc(Q->size*sizeof(Node*));
    Q->head = 0;
    Q->tail = 0;

    Node * workNode = (Node *)malloc(sizeof(Node));
    Node * neighbourNode = (Node *)malloc(sizeof(Node));

    while(R->idx != R->size)
    {
        if (Q->head == Q->tail)
        {
            // Picking nodes from original graph
            workNode = popGraph(graph);
        }
        else
        {
            // Picking iteratively adjacent (neighbour) nodes
            // to the last picked node from the original graph
            workNode = popGraph(Q);
        }

        if(!workNode->inR)
        {
            // R->idx++ happens inside pushArray() call
            pushArray(R, workNode);         
            for(int i=0; i<workNode->degree; i++)
            {
                neighbourNode = workNode->neighbours[i];
                if(!neighbourNode->inQ && !neighbourNode->inR)
                {
                    pushGraph(Q, neighbourNode);
                }
            }
        }
    }
    for(int i=0; i<R->size/2; i++)
    {
        swap(&(R->nodeIdx[i]), &(R->nodeIdx[R->size-i-1]));
    }
    return R;
}

 
