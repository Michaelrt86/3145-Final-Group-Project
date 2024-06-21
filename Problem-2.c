/**
  Write an MPI program that computes a tree-structured global sum. First, write your program for the special case in which comm sz is a power of two.
  Then, after you’ve gotten this version working, modify your program so that it can handle any comm sz.
**/

/* File:  
 *    Problem-2.c
 *
 * Purpose:  
 *    To compute a tree-structured global sum that takes in any comm sz.
 *
 * Compile:
 *    mpicc -o Problem-2 Problem-2.c
 * Run:
 *    mpiexec -n <comm_sz> ./Problem-2 <order of vector>
 *
 * Input:
 *    2 integers
 * Output:
 *    The global sum that is calculated from the input values
 *
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// Function prototypes
void initialize_mpi(int *comm_sz, int *my_rank);
int tree_sum(int my_rank, int comm_sz, int local_value);
void handle_remainder(int *sum, int my_rank, int comm_sz);
void finalize_mpi();

int main(int argc, char **argv)
{
  int comm_sz, my_rank;
  int local_value, global_sum;

  MPI_Init(&argc, &argv);
  initialize_mpi(&comm_sz, &my_rank);

  local_value = my_rank;
  
// Compute tree-structured global sum
  global_sum = tree_sum(my_rank, comm_sz, local_value);
  
// Handle any remaining processes
  handle_remainder(&global_sum, my_rank, comm_sz);
  
// Print global sum from root process
  if (my_rank == 0)
  {
    printf("The global sum is %d\n", global_sum);
  }

  finalize_mpi();
  return 0;
}

// Initialize MPI environment
void initialize_mpi(int *comm_sz, int *my_rank)
{
  MPI_Comm_size(MPI_COMM_WORLD, comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, my_rank);
}

// Perform tree-structured global sum computation
int tree_sum(int my_rank, int comm_sz, int local_val)
{
  int sum = local_val;
  int step;

  for (step = 1; step < comm_sz; step *= 2)
  {
    if (my_rank % (2 * step) == 0)
    {
      int partner = my_rank + step;
      if (partner < comm_sz)
      {
        int received_value;
        MPI_Recv(&received_value, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        sum += received_value;
      }
    }
    else if (my_rank % step == 0)
    {
      int partner = my_rank - step;
      MPI_Send(&sum, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
      break;
    }
  }
  return sum;
}
// Handle any remaining processes after tree reduction
void handle_remainder(int *sum, int my_rank, int comm_sz)
{
  int remaining_processes = comm_sz;

  if (remaining_processes % 2 != 0)
  {
    if (my_rank == remaining_processes - 1)
    {
      MPI_Send(sum, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD);
    }
    if (my_rank == remaining_processes - 2)
    {
      int received_value;
      MPI_Recv(&received_value, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      *sum += received_value;
    }
    remaining_processes--;
  }
}
// Finalize MPI environment
void finalize_mpi()
{
  MPI_Finalize();
}
