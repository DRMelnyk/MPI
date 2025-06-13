#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

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
void mat_mat_serial() {
    cout << "Matrix to matrix multiplication by serial method: " << endl;

    auto start_time = chrono::high_resolution_clock::now(); // Засекаем время

    vector<vector<int>> C = multiply(A, B);
    
    auto end_time = chrono::high_resolution_clock::now(); // Засекаем время

    chrono::duration<double> diff = end_time - start_time;

    print_mat(C);

    cout << "Time taken: " << diff.count() << " seconds" << endl;
}
int main(int argc, char** argv) {
    mat_mat_parallel(argc, argv);
    mat_mat_serial();

    mat_vet_parallel(argc, argv);
    //mat_vet_serial();

    return 0;
}

