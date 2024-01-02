#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <fstream>
#include <string>
#include<random>
#include<chrono>

using namespace std;
using namespace std::chrono;
int Random() { // рандомизация цифр
	random_device rand_dev;
	mt19937 gen(rand_dev());
	uniform_int_distribution<int> random(100000, 999999);
	return random(gen);
}

int main() {
	vector<int> numbers;
	int range = pow(10, 7); //до куда
	int RangeNumbers = 10000;
	unordered_map<int, int> UnicRandomNumbers; // массив значений
	int bytes = (pow(10, 7) + 7) / 8; // выделение памяти
	vector<unsigned char> nums(bytes);
	ofstream f2("Add.txt");
	// Инициализация генератора случайных чисел
	srand(time(NULL));
	for (int i = 0; i < 100000; i++) {
		UnicRandomNumbers[i] = Random();
	}
	
	

	for (const auto& pair : UnicRandomNumbers) {
		numbers.push_back(pair.first);
		f2 << pair.first << endl;
	} // запись значений в 1 файл Add
	f2.close();
	ifstream f("Add.txt");
	ofstream f1("Out.txt");
	string line;

	int BitId, ByteInd, SetBit;
	while (getline(f, line)) {
		BitId = stoi(line); //Перевод из string в int
		ByteInd = BitId / 8; // Индекс байта в котором находится бит
		SetBit = BitId % 8; // Смещение внутри байта для доступа к биту
		nums[ByteInd] |= (1 << SetBit); // Устанавливаем бит в байте
	}

	f.close();
	auto start = high_resolution_clock::now();
	for (int byteInd = 0; byteInd < bytes; byteInd++) { // запись в файл отсортированных чисел
		unsigned char byte = nums[byteInd];
		for (int bitId = 0; bitId < 8; bitId++) {
			if ((byte & (1 << bitId)) != 0) {
				f1 << (byteInd * 8 + bitId) << endl;
			}
		}
	}
	auto stop = high_resolution_clock::now();
	f1.close();
	auto duration = duration_cast<milliseconds>(stop - start);
	cout << "Time in ms: " << duration.count() << endl << "Mb: " << nums.capacity() / (pow(2, 20));
}
