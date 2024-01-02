#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

// Функция для проверки, все ли элементы квадратного блока равны нулю
bool isBlockZero(const vector<vector<int>>& matrix, int row, int col, int size) {
    for (int i = row; i < row + size; i++) {
        for (int j = col; j < col + size; j++) {
            if (matrix[i][j] != 0)
                return false;
        }
    }
    return true;
}

// Функция для нахождения квадратного блока размера size, состоящего из нулей
bool findZeroBlock(const vector<vector<int>>& matrix, int size) {
    int m = matrix.size();
    int n = matrix[0].size();

    for (int i = 0; i <= m - size; i++) {
        for (int j = 0; j <= n - size; j++) {
            if (isBlockZero(matrix, i, j, size)) {
                cout << "Квадратный блок найден с координатами: (" << i << ", " << j << ")" << endl;
                cout << "Визуализация квадратного блока:" << endl;
                for (int k = i; k < i + size; k++) {
                    for (int l = j; l < j + size; l++) {
                        cout << matrix[k][l] << " ";
                    }
                    cout << endl;
                }
                return true;
            }
        }
    }

    cout << "Квадратный блок не найден." << endl;
    return false;
}

// Функция для перебора всех значений и оценки скорости
/*void bruteForceEvaluation(const vector<vector<int>>& matrix, int size) {
    auto start = chrono::high_resolution_clock::now(); // Запускаем таймер

    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            if (matrix[i][j] == 0)
                findZeroBlock(matrix, size);
        }
    }

    auto end = chrono::high_resolution_clock::now(); // Останавливаем таймер
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count(); // Вычисляем длительность в миллисекундах

    cout << "Время выполнения перебора всех значений: " << duration << " мс" << endl;
}*/

int main() {
    setlocale(0, "");
    int M, N;
    cout << "Введите размеры массива (M и N): ";
    cin >> M >> N;

    vector<vector<int>> matrix(M, vector<int>(N));
    cout << "Введите элементы массива (0 или 1):" << endl;

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cin >> matrix[i][j];
        }
    }

    int blockSize;
    cout << "Введите размер квадратного блока: ";
    cin >> blockSize;

    findZeroBlock(matrix, blockSize);
    //bruteForceEvaluation(matrix, blockSize);

    return 0;
}