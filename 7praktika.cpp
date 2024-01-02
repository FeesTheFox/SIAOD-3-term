#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

// ������� ��� ��������, ��� �� �������� ����������� ����� ����� ����
bool isBlockZero(const vector<vector<int>>& matrix, int row, int col, int size) {
    for (int i = row; i < row + size; i++) {
        for (int j = col; j < col + size; j++) {
            if (matrix[i][j] != 0)
                return false;
        }
    }
    return true;
}

// ������� ��� ���������� ����������� ����� ������� size, ���������� �� �����
bool findZeroBlock(const vector<vector<int>>& matrix, int size) {
    int m = matrix.size();
    int n = matrix[0].size();

    for (int i = 0; i <= m - size; i++) {
        for (int j = 0; j <= n - size; j++) {
            if (isBlockZero(matrix, i, j, size)) {
                cout << "���������� ���� ������ � ������������: (" << i << ", " << j << ")" << endl;
                cout << "������������ ����������� �����:" << endl;
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

    cout << "���������� ���� �� ������." << endl;
    return false;
}

// ������� ��� �������� ���� �������� � ������ ��������
/*void bruteForceEvaluation(const vector<vector<int>>& matrix, int size) {
    auto start = chrono::high_resolution_clock::now(); // ��������� ������

    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            if (matrix[i][j] == 0)
                findZeroBlock(matrix, size);
        }
    }

    auto end = chrono::high_resolution_clock::now(); // ������������� ������
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count(); // ��������� ������������ � �������������

    cout << "����� ���������� �������� ���� ��������: " << duration << " ��" << endl;
}*/

int main() {
    setlocale(0, "");
    int M, N;
    cout << "������� ������� ������� (M � N): ";
    cin >> M >> N;

    vector<vector<int>> matrix(M, vector<int>(N));
    cout << "������� �������� ������� (0 ��� 1):" << endl;

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cin >> matrix[i][j];
        }
    }

    int blockSize;
    cout << "������� ������ ����������� �����: ";
    cin >> blockSize;

    findZeroBlock(matrix, blockSize);
    //bruteForceEvaluation(matrix, blockSize);

    return 0;
}