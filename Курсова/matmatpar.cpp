#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int COLS = 4, ROWS = 3;
vector<vector<int>> A = {
            {0, 1, 3, 8},
            {4, 1, -3, -6},
            {1, 3, -2, 6}
};
vector<vector<int>> B = {
            {5, 4,3},
            {1,1,2},
            {3,-3,-6},
            {5,-3,6}
};
vector<int> v = { 1, 3, 0, -3 };

void print_mat(vector<vector<int>>& A) {
    cout << "Matrix:";
    for (vector<int> temp_v : A)
    {
        cout << endl;
        for (int temp : temp_v)
        {
            cout << temp << "\t";
        }
    }
    cout << endl;
}
vector<vector<int>> multiply(vector<vector<int>>& A, vector<vector<int>>& B) {

    vector<vector<int>> C(ROWS, vector<int>(ROWS, 0));

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < ROWS; j++) {
            for (int k = 0; k < COLS; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}
void mat_mat_parallel(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Bcast(&A[0][0], ROWS * COLS, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&B[0][0], COLS * ROWS, MPI_INT, 0, MPI_COMM_WORLD);

    double startTime;

    if (rank == 0) {
        cout << "Matrix to matrix multiplication by parallel method: " << endl;
        startTime = MPI_Wtime();
    }

    vector<vector<int>> C = multiply(A, B);

    if (rank == 0) {
        double endTime = MPI_Wtime();
        print_mat(C);
        cout << "Total time: " << endTime - startTime << endl;
    }
    MPI_Finalize();
}

