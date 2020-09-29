#include <stdlib.h>
#include "../inc/rcm.h"


int main(int argc, char *argv[])
{
    // Enter matrix size and density
    int size;
    double density;
    printf( "Enter matrix size :");
    scanf("%d", &size);
    printf( "Enter density (%%) :");
    scanf("%lf", &density);
   
    struct timeval start, end;
    
    // Create a random Sparse Matrix
    int *arr = generateSparseMatrix(size, density);
    
    int band;
    // band = bandwith(arr, size);
    // printf("Initial bandwidth: %d\n",band);

    ///////////////////////////////////////////////////////
    //                   OPTIONAL STEP                   //
    // Save the sparseMatrix in .csv or txt file format  //
    // for Matlab validation tests                       //
    ///////////////////////////////////////////////////////
    // saveCsv(arr, size, 1);
    // saveTxt(arr, size);
    //////////////////////////////////////////////////////
    
    gettimeofday(&start, NULL);
    // Calculate permutations matrix
    Array * R = rcm(arr, size);
    gettimeofday(&end, NULL);

    double exetime = (double)((end.tv_usec - start.tv_usec)/1.0e6 + end.tv_sec - start.tv_sec);
    printf("\t\t\tRCM Finished\n");
    printf("N: %d\tDensity: %.1lf%%\tRCM Execution Time: %lf\n",size,density,exetime);
    

    //////////////////////////////////////////////////////////
    //                   OPTIONAL STEPS                     //
    // Since R is calculated the main purpose of the        //
    // assignment is done. The next steps generate the      //
    // final graph in *finalGraph, the final sparse matrix  //
    // in *arr and the bandwidth of the final sparse matrix.//
    // Also, the produced sparse matrix is saved in a .csv  //
    // file. The above steps are time consuming for big     //
    // matrix sizes. Uncomment to execute.                  //
    //////////////////////////////////////////////////////////
    
    // // Re-generate the initial graph, since the first was 
    // // changed from function "rcmSequential"
    // printf("Recreating the initial graph. This might take a while...\n");
    // Graph * initialGraph = initializeGraph(arr, size);
     
    // // Create the final graph from the initial graph and 
    // // the permutation matrix R
    // Graph * finalGraph = finalizeGraph(initialGraph, R);
    // printf("Constructing the final graph. This might take a while...\n");
    
    // // Generate the final Sparse Matrix  
    // graphToSparseMatrix(finalGraph, size, arr);
    // printf("Creating the final sparse matrix. This might take a while...\n");
    
    // // Save the final Sparse Matrix in a .csv files
    // saveCsv(arr, size, 0);
    
    // // Calculate the bandwidth of the transformed Sparse Matrix
    // band = bandwith(arr, size);
    // printf("Final bandwidth: %d\n", band);
    // free(finalGraph);
    // free(initialGraph);

    /////////////////////////////////////////////////////////////

    // Free allocated memory
    free(arr);
    return 0;
}





