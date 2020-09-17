#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/rcm.h"
#include <sys/time.h>

// This function generates the permutation Array R.
// R has the form of an Array struct.
Array * rcmSequential(Graph * graph)
{
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
            workNode = popGraph(graph);
        }
        else
        {
            workNode = popGraph(Q);
        }
        if(!workNode->inR)
        {
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

 
