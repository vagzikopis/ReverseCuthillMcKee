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
    int band;
    
    band = bandwith(arr, size);
    printf("Init band: %d\n", band);
    saveCsv(arr, size, 1);
    saveTxt(arr, size);

///////////////////////////////////////////////////////////////////
    gettimeofday(&start, NULL);
    // Generate the initial graph
    Graph * graph = initializeGraph(arr, size); 
    // Calculate permutations matrix
    Array * R = rcm(graph);
    gettimeofday(&end, NULL);
    
    // Re-generate the initial graph, since the first was 
    // changed from function "rcmSequential"
    Graph * initialG = initializeGraph(arr, size);

    // Create the final graph from the initial graph and 
    // the permutation matrix R
    Graph * finalG = finalGraph(initialG, R);
    // // Print the final Sparse Matrix  
    graphToSparseMatrix(finalG, size, arr);
    
    //Bandwith output using Sequential method
    band = bandwith(arr, size);
    printf("Seq band: %d\n", band);
    
    printf("N: %d\tDensity: %.1lf%%\tSequential RCM Execution Time: %lf\n",size,density,
            (double)((end.tv_usec - start.tv_usec)/1.0e6 + 
            end.tv_sec - start.tv_sec));

///////////////////////////////////////////////////////////////////
    // array is altered by bandwith() 
    // re-read it from .txt
    char path[50] = "../sparseMatrices/input_size";
    char name[8];
    char type[8] = ".txt";
    sprintf(name, "%d", size);
    strcat(path, name);
    strcat(path, type);
    arr = readTxt(path,size);
    
    gettimeofday(&start, NULL);
    // Generate the initial graph
    Graph * graph_p = parallelInitGraph(arr, size); 
    // Calculate permutations matrix
    Array * R_p = rcm(graph_p);
    gettimeofday(&end, NULL);

    // Re-generate the initial graph, since the first was 
    // changed from function "rcmSequential"
    Graph * initialG_p = initializeGraph(arr, size);
    
    // Create the final graph from the initial graph and 
    // the permutation matrix R
    Graph * finalG_p = finalGraph(initialG_p, R);
    // Print the final Sparse Matrix  
    graphToSparseMatrix(finalG_p, size, arr);

    //Bandwith output using Sequential method
    band = bandwith(arr, size);
    printf("Par band: %d\n", band);

    printf("N: %d\tDensity: %.1lf%%\tParallel RCM Execution Time: %lf\n",size,density,
            (double)((end.tv_usec - start.tv_usec)/1.0e6 + 
            end.tv_sec - start.tv_sec));
    
    // // Free allocated memory
    free(graph);
    free(initialG);
    free(initialG_p);
    free(finalG);
    free(finalG_p);
    free(R);
    free(R_p);
    free(arr);
    return 0;
}





