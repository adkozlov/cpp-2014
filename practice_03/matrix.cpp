#include <iostream>

int main() {
    int n, m;
//    std::cin >> n >> m;
    n = 5;
    m = 4;

    int *temp = new int[n * m];
    int **matrix = new int*[n];

    for (int i = 0; i < n; i++) {
        matrix[i] = &temp[i * m];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix[i][j] = i * m + j;
        }
    }

    std::cout << matrix[4][3] << std::endl;

    delete[] matrix;
    delete[] temp;

    return 0;
}
