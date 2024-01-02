
#include <iostream>
#include <vector>
using namespace std;

unsigned char ConvBitSeq(vector<int>& numbers, vector<int>& count) {
    unsigned char bitSequence = 0;
    for (int num : numbers) {//ѕроходит по массиву чисел
        bitSequence |= (1 << num); //ѕроизводит сдвиг до num-го бита и усанавливает там 1
        count[num] += 1; // подсчитывает количество вхождений числа
    }
    return bitSequence;
}


vector<int> ConvSortNumb(unsigned char bitSequence, vector<int>& count) {
    vector<int> sortedNumbers;
    for (int i = 0; i < 8; ++i) {
        if ((bitSequence & (1 << i)) != 0) { // ѕровер€ет стоит ли  единица в заданном бите, если да то:
            for (int j = 0; j < count[i]; j++) {
                sortedNumbers.push_back(i); // добавл€ем в vector число, причЄм столько раз оно повтор€лось
            }
        }
    }
    return sortedNumbers;
}

int main() {
    vector<int> counter = { 0, 0, 0, 0, 0, 0, 0, 0 };
    vector<int> inputNumbers = { 1, 0, 5, 7, 2, 4, 6, 4, 5, 2, 1, 0,7,7,4,5, 0, 5, 7, 2,
    4, 6, 3, 5, 2, 1, 0,7,7,4,5,7, 1, 0, 5, 7, 3, 4, 6, 2, 5, 2, 1, 0,7,7,4,5,7,
     1, 0, 5, 7, 2, 4, 6, 4, 5, 2, 1,6, 6, 3 ,3 };
    unsigned char bitSequence = ConvBitSeq(inputNumbers, counter);
    vector<int> sortedNumbers = ConvSortNumb(bitSequence, counter);

    for (int num : sortedNumbers) {
        cout << num << " ";
    }
    cout << endl;

    for (int index : counter) {
        cout << index << " ";
    }
}

