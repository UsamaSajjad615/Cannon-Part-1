# Parallel Matrix Multiplication with Cannon's Algorithm

This project implements **Cannon's Algorithm** for matrix multiplication using **MPI (Message Passing Interface)**. The program distributes **n × n** matrices across **p = n²** processors, where each process computes a partial result.

## **Features**
- Uses **MPI** for parallel matrix multiplication.
- Implements **Cannon’s Algorithm** for distributed computation.
- Supports **variable matrix sizes** via `#define MATRIX_SIZE`.
- Ensures correctness by requiring **p = n²** processors.
- Uses **random values** for matrix initialization.

## **Requirements**
- A system with **MPI (Message Passing Interface)** installed.
- **Linux or Windows** with an MPI implementation (e.g., OpenMPI, MPICH).

## **Installation**
Clone the repository:
```sh
git clone https://github.com/yourusername/Cannon-MPI.git
cd Cannon-MPI


## Compilation
Compile using mpicc:
mpicc -o cannon new_Cannon_Task_A.c

## Execution
Run the program with n² processes:
mpirun -np 4 ./cannon
To test other sizes, modify #define MATRIX_SIZE and ensure -np matches MATRIX_SIZE²

## Limitations
Only works when number of processes (p) = MATRIX_SIZE².
Does not use an MPI Cartesian grid, which could optimize communication.
Needs explicit MPI barriers to avoid race conditions.

Made with 💖 by Usama Sajjad