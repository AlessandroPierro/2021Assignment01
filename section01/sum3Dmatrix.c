#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mpi.h"

int main(int argc, char *argv[])
{

    // ---- Variable definition --- //
    int rank, size, f = 0;
    double start_time, end_time, time_mean = 0;

    // ---- MPI initialization ---- //
    MPI_Comm MPI_GRID_WORLD;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // ---- Read matrix dimension from command line --- //
    int n_x = atoi(argv[1]);
    int n_y = atoi(argv[2]);
    int n_z = atoi(argv[3]);
    int total_dimension = n_x * n_y * n_z;
    int padded_dimension = total_dimension + total_dimension % size;

    // ---- Read domain decomposition from command line --- //
    int n_x_proc = atoi(argv[4]);
    int n_y_proc = atoi(argv[5]);
    int n_z_proc = atoi(argv[6]);

    // ---- Read the number of iterations from command line --- //
    int n_iterations = atoi(argv[7]);

    assert(n_x_proc * n_y_proc * n_z_proc == size); // Check if the number of processors is correct
    int dims[3] = {n_x_proc, n_y_proc, n_z_proc};

    // ---- Create Cartesian topology ---- //
    MPI_Cart_create(MPI_COMM_WORLD, 3, dims, &f, 1, &MPI_GRID_WORLD);
    MPI_Comm_rank(MPI_GRID_WORLD, &rank);
    int coord[3];
    MPI_Cart_coords(MPI_GRID_WORLD, rank, 3, coord);


    // ---- Allocate memory for the matrix ---- //
    double *A = (double *)malloc(padded_dimension * sizeof(double));
    double *B = (double *)malloc(padded_dimension * sizeof(double));

    // ---- Initialize A and B with random double ---- //
    srand(rank);
    for (int i = 0; i < total_dimension; i++)
    {
        A[i] = (double)rand() / (double)RAND_MAX;
        B[i] = (double)rand() / (double)RAND_MAX;
    }

    // ---- Check if padding is needed ---- //
    int local_dimension = padded_dimension / size;

    // ---- Allocate memory for local matrices ---- //
    double *A_1d = (double *)malloc(local_dimension * sizeof(double));
    double *B_1d = (double *)malloc(local_dimension * sizeof(double));
    
    // ---- Scatter A and B ---- //
    MPI_Scatter(A, local_dimension, MPI_DOUBLE, A_1d, local_dimension, MPI_DOUBLE, 0, MPI_GRID_WORLD);
    MPI_Scatter(B, local_dimension, MPI_DOUBLE, B_1d, local_dimension, MPI_DOUBLE, 0, MPI_GRID_WORLD);

    // ---- Get time from MPI_Wtime --- //
    start_time = MPI_Wtime();

    // ---- Compute the sum of the local matrices ---- //
    for (int i = 0; i < local_dimension; i++)
    {
        B_1d[i] = A_1d[i] + B_1d[i];
    }
    
    // ---- Get time from MPI_Wtime --- //
    end_time = MPI_Wtime();

    // ---- Gather local matrices ---- //
    MPI_Gather(A_1d, local_dimension, MPI_DOUBLE, A, local_dimension, MPI_DOUBLE, 0, MPI_GRID_WORLD);

    // ---- Print time on processor 0 ---- //
    if (rank == 0)
    {
        printf("%d,%d,%f\n", size, n_x*n_y*n_z, end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}