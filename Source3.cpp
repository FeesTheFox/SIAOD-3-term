#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <set>
#include <chrono>
#include <algorithm>

using namespace std;

struct TaxRecord
{
    long long INN = 0;
    string surname;
    string number;
};


struct Tablet
{
    long long key;
    int offset; //Смещение (работа ползунком  блин)))
};

unsigned long long RandPhoneNumber() //Случайный номер телефона
{
    long long min = 79850000000;
    long long max = 79999999999;
    return min + rand() % (max - min + 1);
}

string RandSurname() 
{
    string surnames[10] = { "Казаков", "Дженбаз", "Руденко", "Наркунас", "Евремова", "Зайцева", "Сидорёнок", "Гиль", "Зильберглейт", "Алыхова" };
    int RandomIndex = rand() % 10;
    return surnames[RandomIndex];
}

unsigned long long Randomizer()
{
    long long min = 1000000000;
    long long max = 9999999999;
    return min + rand() % (max - min + 1);
}


void Read(const string& file, int offset, TaxRecord& record)
{
    ifstream fileT(file, ios::binary);
    if (!fileT)
    {
        cout << "Бинарный файл не удалось открыть для чтения." << endl;
        exit(1);
    }
    cout << "Бинарный файл успешно открыт для чтения." << endl << endl;

    fileT.seekg(offset * sizeof(TaxRecord), ios::beg);

    if (fileT.read(reinterpret_cast<char*>(&record), sizeof(TaxRecord)))
    {
        if (record.INN != 0) {
            cout << "Запись ИНН найдена!" << endl;
            cout << "Номер ИНН: " << record.INN << endl;
            cout << "Фамилия: " << record.surname << endl;
            cout << "Номер телефона: +" << record.number << endl;
        }
    }
    else
    {
        cout << endl << "Бинарный файл не удалось прочитать" << endl;
    }
    fileT.close();
}

//mid = left + ((key - a.left) *(right - left))/(a.right - a.left)
//интерполяционный поиск
int InterpolationSearch(const vector <Tablet> TabletC, long long key)
{
    int left = 0;  // левая граница поиска (будем считать, что элементы массива нумеруются с нуля) 
    int  right = TabletC.size() - 1; // правая граница поиска 


    while (TabletC[left].key < key and key < TabletC[right].key) {
        int mid = left + (key - TabletC[left].key) * (right - left) / (TabletC[right].key - TabletC[left].key);  // индекс элемента, с которым будем проводить сравнение 
        if (TabletC[mid].key < key) {
            left = mid + 1;
        }
        else if (TabletC[mid].key > key) {
            right = mid - 1;
        }
        else
            return mid;
    }
    if (TabletC[left].key == key) {
        return left;
    }
    else if (TabletC[right].key == key) {
        return right;
    }
    else {
        return -1; // если такого элемента в массиве нет 
    }
    
}





int main()
{
    setlocale(LC_ALL, "Rus");
    srand(static_cast<unsigned int>(time(nullptr)));


    ofstream fileText("text.txt");
    if (!fileText)
    {
        cout << "Текстовый файл не удалось открыть для записи." << endl;
        return 1;
    }
    cout << "Текстовый файл успешно открыт для записи." << endl << endl;

    ofstream fileBinaryOUT("text.bin", ios::binary);
    if (!fileBinaryOUT) {
        cout << "Бинарный файл не удалось открыть для записи." << endl;
        return 1;
    }
    cout << "Бинарный файл успешно открыт для записи." << endl << endl;

    int recordcount = 5;
    set<int> unique; //Массив уникальных значений

    TaxRecord record;

    for (int i = 0; i < recordcount; i++)
    {
        do {
            record.INN = Randomizer();
        } while (unique.count(record.INN) > 0);
        unique.insert(record.INN);
        record.surname = RandSurname();
        record.number = to_string(RandPhoneNumber());

        fileText << record.INN << ' ' << record.surname << ' ' << record.number << '\n';

        // Запись в бинарный файл и индексация записи
        fileBinaryOUT.write(reinterpret_cast<const char*>(&record), sizeof(record));
    }
    cout << "Генерация текстового и бинарного файла завершена." << endl << endl;




   /*size_t bytes = sizeof(TaxRecord);
   cout << "Размер записи в байтах: " << bytes << endl;
   cout << "Количество записей: " << recordcount << endl << endl;*/

    fileText.close();
    fileBinaryOUT.close();



    //Файл должен быть открыт в бинарном виде для чтения
    ifstream fileBinaryIN("text.bin", ios::binary);
    if (!fileBinaryIN)
    {
        cout << "Бинарный файл не удалось открыть для чтения " << endl;
        return 1;
    }
    cout << "Файл открыт для чтения" << endl;

    //Заполнение таблицы
    vector <Tablet> TabletC;
    int offset = 0;
    while (fileBinaryIN.read(reinterpret_cast<char*>(&record), sizeof(record)))
    {
        Tablet container;
        container.key = record.INN; //Заполнение ключей
        container.offset = offset; //Создаём ссылку на ключ 
        offset++;
        TabletC.push_back(container); //Добавление значений в таблицу
    }
    fileBinaryIN.close();


    sort(TabletC.begin(), TabletC.end(), [](const Tablet& a, const Tablet& b) {return a.key < b.key; }); //Сортировка таблицы

    cout << "!----Поиск ИНН----!" << endl << endl;
    long long key;
    cout << "Введите ИНН-номер (10 цифр): ";
    cin >> key;


    auto start = chrono::high_resolution_clock::now();

    int result = InterpolationSearch(TabletC, key);
    if (result == -1)
    {
        cout << "Элемент не найден\n";
        return 0;
    }

    //Линейный поиск
    //while (fileBinaryIN.read(reinterpret_cast<char*>(&record), sizeof(record)))
    //{
    //    if (record.INN == key)
    //    {
    //        cout << endl << " ИНН найден" << endl;
    //        cout << " ИНН " << record.INN << endl;
    //        cout << " Фамилия " << record.surname << endl;
    //        cout << " Номер телефона +" << record.number << endl;
    //        //Постусловие: ключ найден, функция должна вернуть 0 и вывести информацию
    //      
    //        fileBinaryIN.close();
    //        auto end = chrono::high_resolution_clock::now(); 
    //        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    //        cout << " Время выполнения: " << duration.count() << " [мкс]" << endl;
    //        return 0;
    //    }
    //}
    // :3 //
    //Постусловие: ключ не найден, функция должна вернуть 0 и вывести сообщение
    /*cout << "Запись с номером " << key << " не была найдена." << endl;
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Время выполнения поиска: " << duration.count() << " секунд." << endl;
    fileBinaryIN.close();*/

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
    Read("text.bin", TabletC[result].offset, record);
    cout << "Время выполнения поиска: " << duration.count() << " наносекунд." << endl;

        


    return 0;
}