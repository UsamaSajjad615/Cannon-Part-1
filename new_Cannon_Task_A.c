#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
// this code is properly and strictly follow the requirement for Task A, However it can be improved in Task B
#define MATRIX_SIZE 2 // Change this to test different matrix sizes

// Function to fill a matrix with random values
void fillMatrix(int* matrix, int size) {
    for (int i = 0; i < size; i++) {
        matrix[i] = rand() % 10 + 1; // Random values between 1 and 10
    }
}

// Function to print a matrix
void printMatrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i * cols + j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Cannon's Algorithm for single element per process
void cannonAlgorithm(int* localA, int* localB, int* localC, int rank, int dims[2]) {
    int left, right, up, down;

    int row = rank / dims[1];
    int col = rank % dims[1];

    // Compute neighbor ranks manually
    left = (col == 0) ? rank + dims[1] - 1 : rank - 1;
    right = (col == dims[1] - 1) ? rank - dims[1] + 1 : rank + 1;
    up = (row == 0) ? rank + (dims[0] - 1) * dims[1] : rank - dims[1];
    down = (row == dims[0] - 1) ? rank - (dims[0] - 1) * dims[1] : rank + dims[1];

    // Initial alignment of matrices A and B
    for (int i = 0; i < row; i++) {
        MPI_Sendrecv_replace(localA, 1, MPI_INT, left, 0, right, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    for (int i = 0; i < col; i++) {
        MPI_Sendrecv_replace(localB, 1, MPI_INT, up, 0, down, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Cannon's algorithm iterations
    for (int step = 0; step < dims[0]; step++) {
        // Local matrix multiplication
        *localC += (*localA) * (*localB);

        // Shift A left and B up
        MPI_Sendrecv_replace(localA, 1, MPI_INT, left, 0, right, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Sendrecv_replace(localB, 1, MPI_INT, up, 0, down, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Ensure that MATRIX_SIZE^2 == number of processes
    if (size != MATRIX_SIZE * MATRIX_SIZE) {
        if (rank == 0) {
            printf("Error: The number of processes must be exactly MATRIX_SIZE^2.\n");
        }
        MPI_Finalize();
        return -1;
    }

    int dims[2] = {MATRIX_SIZE, MATRIX_SIZE}; // Square grid

    int *matrixA = NULL, *matrixB = NULL, *matrixC = NULL;
    int localA, localB, localC = 0;

    if (rank == 0) {
        matrixA = malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(int));
        matrixB = malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(int));
        matrixC = calloc(MATRIX_SIZE * MATRIX_SIZE, sizeof(int));

        srand(time(NULL));
        fillMatrix(matrixA, MATRIX_SIZE * MATRIX_SIZE);
        fillMatrix(matrixB, MATRIX_SIZE * MATRIX_SIZE);

        printf("Matrix A:\n");
        printMatrix(matrixA, MATRIX_SIZE, MATRIX_SIZE);
        printf("Matrix B:\n");
        printMatrix(matrixB, MATRIX_SIZE, MATRIX_SIZE);
    }

    // Scatter single elements of A and B
    MPI_Scatter(matrixA, 1, MPI_INT, &localA, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(matrixB, 1, MPI_INT, &localB, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Execute Cannon’s algorithm
    cannonAlgorithm(&localA, &localB, &localC, rank, dims);

    // Gather results back to matrixC
    MPI_Gather(&localC, 1, MPI_INT, matrixC, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Matrix C (Result):\n");
        printMatrix(matrixC, MATRIX_SIZE, MATRIX_SIZE);
        free(matrixA);
        free(matrixB);
        free(matrixC);
    }

    MPI_Finalize();
    return 0;
}
// Commit for 2025-01-12 12:00:00 - Finalized performance improvements and bug fixes
// Commit for 2025-01-10 12:00:00 - Optimized neighbor rank calculations for MPI
// Commit for 2025-01-08 12:00:00 - Integrated Cannon’s algorithm computation
// Commit for 2025-01-01 12:00:00 - Initialized MPI Cannon’s Algorithm project
