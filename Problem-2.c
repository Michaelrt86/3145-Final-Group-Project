/**
  Write an MPI program that computes a tree-structured global sum. First, write your program for the special case in which comm sz is a power of two. 
  Then, after you’ve gotten this version working, modify your program so that it can handle any comm sz.
**/ 

/* File:
 *    Problem-2.c
 *
 * Purpose:
 *    To compute a tree-structured global sum that takes in any comm_sz.
 *
 * Compile:
 *    mpicc -o TreeSum Problem-2.c
 * Run:
 *    mpiexec -n <comm_sz> ./Problem-2.c
 *
 * Input:
 *    2 integers
 * Output:
 *    The global sum of all ranks
 *
 */


#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, comm_sz;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    
    // Get the local value (rank) for each process
    int local_value = rank;
    int partner;
    int level_size = 1;
    
    // Tree-structured sum
    while (level_size < comm_sz) {
        if (rank % (2 * level_size) == 0) {
            partner = rank + level_size;
            if (partner < comm_sz) {
                int received_value;
                MPI_Recv(&received_value, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_value += received_value;
            }
        } else {
            partner = rank - level_size;
            if (partner >= 0) {
                MPI_Send(&local_value, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
            }
            break; 
        }
        level_size *= 2;
    }
    

    if (rank == 0) {
        int received_value;
        for (int i = 1; i < comm_sz; i++) {
            MPI_Recv(&received_value, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            local_value += received_value;
        }
    } else if (partner < comm_sz) {
        MPI_Send(&local_value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
   
    if (rank == 0) {
        printf("Global sum: %d\n", local_value);
    }
    
    MPI_Finalize();
    return 0;
}

