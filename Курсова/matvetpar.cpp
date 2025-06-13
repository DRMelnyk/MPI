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
void print_mat(vector<int>& A) {
    cout << "Vector:" << endl;
    for (int temp : A) {
        cout << temp << "\t";
    }
    cout << endl;
}
vector<int> multiply(vector<vector<int>>& A, vector<int>& v) {
    vector<int> result(ROWS, 0);

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            result[i] += A[i][j] * v[j];
        }
    }

    return result;
}
void mat_vet_parallel(int argc, char** argv) {
    int size, rank;
    double start;
    MPI_Init(&argc, &argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int local_matrix[COLS]{};
    MPI_Scatter(&À.data(), COLS, MPI_INT, &local_matrix, COLS, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(v.data(), COLS, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank == 0)
        start = MPI_Wtime();
    
    int local_result = 0;
    for (int i = 0; i < COLS; i++) {
        local_result += local_matrix[i] * vec[i];
    }
    
    MPI_Gather(&local_result, 1, MPI_INT, &result, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        cout << "Matrix to vector multiplication by parallel method: " << endl;
        double end = MPI_Wtime();
        cout << "Vector: " << endl;
        for (int temp : result)
            cout << temp << "\t";
        cout << "\nTime: " << end - start << " seconds" << endl;
    }
    
    MPI_Finalize();
}

int main(int argc, char** argv) {
    mat_vet_parallel(argc, argv);
    return 0;
}

