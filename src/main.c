#include <stdlib.h>
#include "../inc/rcm.h"


int main(int argc, char *argv[])
{

    // Enter matrix size and density
    int size;
    double density;
    
    printf( "Enter matrix size :");
    scanf("%d", &size);
    printf( "Enter density (%%):");
    scanf("%lf", &density);

    struct timeval start, end;
    int *arr = (int *)malloc(size*size*sizeof(int));
    // Create a random Sparse Matrix
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
    // saveCsv(arr, size, 1);
    // saveTxt(arr, size);

    gettimeofday(&start, NULL);
    // Generate the initial graph
    Graph * graph = initializeGraph(arr, size); 
    // Calculate permutations matrix
    Array * R = rcmSequential(graph);
    gettimeofday(&end, NULL);

    // Re-generate the initial graph, since the first was 
    // changed from function "rcmSequential"
    // Graph * initialG = initializeGraph(arr, size);

    // Create the final graph from the initial graph and 
    // the permutation matrix R
    // Graph * finalG = finalGraph(initialG, R);
    printf("N: %d\tDensity: %.1lf%%\tSequential RCM Execution Time: %lf\n",size,density,
            (double)((end.tv_usec - start.tv_usec)/1.0e6 + 
            end.tv_sec - start.tv_sec));

    gettimeofday(&start, NULL);
    // Generate the initial graph
    Graph * graph_p = parallelInitGraph(arr, size); 
    // Calculate permutations matrix
    Array * R_p = rcmParallel(graph_p);
    gettimeofday(&end, NULL);

    printf("N: %d\tDensity: %.1lf%%\tParallel RCM Execution Time: %lf\n",size,density,
            (double)((end.tv_usec - start.tv_usec)/1.0e6 + 
            end.tv_sec - start.tv_sec));
   
    // Print the final Sparse Matrix  
    // graphToSparseMatrix(finalG, size, arr);
    // saveCsv(arr, size, 0);

    // Free allocated memory
    free(graph);
    //free(initialG);
    //free(finalG);
    free(R);
    free(arr);
}





