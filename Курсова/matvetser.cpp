#include <chrono>
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

void mat_vet_serial() {
    cout << "Matrix to vector multiplication by serial method: " << endl;
    auto start_time = chrono::high_resolution_clock::now();

    vector<int> C = multiply(A, v);

    auto end_time = chrono::high_resolution_clock::now(); 
    chrono::duration<double> diff = end_time - start_time;

    print_mat(C);

    cout << "Time taken: " << diff.count() << " seconds" << endl;
} 

int main() {
    mat_vet_serial();
    return 0;
}

