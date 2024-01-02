#include <unordered_map>//���-������ �������, �������� ���� � ������, ����������� �� ���� (������ �����������)
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <list>
#include <set>

using namespace std;


//��������� ������ - ���� ��������: ����, ������� 
struct DateOfBirth
{
    unsigned long int number = 0;
    string surname;
};


//��������� ������ � ���-�������
struct HashTableEntry
{
    unsigned long int key;
    int index;
};


//����� ������, ��������� �� 8 �����
unsigned long int Randomizer()
{
    unsigned long int min = 10000000;
    unsigned long int max = 99999999;
    return min + rand() % (max - min + 1);
}


//���-������� ������� �������
int HashFun(unsigned long int key, int tableSize)
{
    return key % tableSize;
}


//������� ����� � �������
void Insert(unordered_map<int, list<HashTableEntry>>& hashTable, unsigned long int key, int tableSize, int index)
{
    HashTableEntry entry;
    entry.key = key;
    entry.index = index;
    int hash = HashFun(key, tableSize);
    hashTable[hash].push_back(entry);
}


//�������� ����� �� ������� � �����
void Remove(unordered_map<int, list<HashTableEntry>>& hashTable, DateOfBirth& policy, const string binary, unsigned long int key, int tableSize)
{
    int hash = HashFun(key, tableSize);
    bool corr = false;

    auto& entries = hashTable[hash];//��������� ������ �� ������ (���� �������)

    list<HashTableEntry> tempEntries;//��������� �������

    for (auto it = entries.begin(); it != entries.end(); ++it)
    {
        if (it->key != key)
        {
            tempEntries.push_back(*it);//�������� �� ���-�������
        }
        else if (it->key == key)
        {
            //�������� �� �����
            corr = true;

            cout << endl << "������� ������� �������� ������ �� �����" << endl;
            ofstream file(binary, ios::binary | ios::in | ios::out);
            if (file)
            {
                file.seekp(it->index * sizeof(DateOfBirth), ios::beg);

                //������ ������
                DateOfBirth empty;
                empty.number = 0;
                empty.surname = "";
                file.write(reinterpret_cast<char*>(&empty), sizeof(DateOfBirth));
                cout << "   ���������� ������ ������� �� �����" << endl;
            }
            file.close();
        }
    }
    entries = tempEntries;
    if (corr == false)
        cout << endl << "����� �� ������� ������� �� �����" << endl;
}


//����� ����� � ������� � �����
void Find(const unordered_map<int, list<HashTableEntry>>& hashTable, DateOfBirth& policy, const string& binary, unsigned long int key, int tableSize)
{
    int hash = HashFun(key, tableSize);
    bool corr = false;

    //�������� ������������� ����� � �������
    auto it = hashTable.find(hash);
    if (it != hashTable.end())
    {
        //��������� ������ �� ���� �������
        auto& entries = it->second;

        for (const HashTableEntry& entry : entries)
        {
            if (entry.key == key)
            {
                cout << endl << "���� �������� ������ � �������" << endl;
                ifstream file(binary, ios::binary);
                if (file)
                {
                    cout << "   �������� ������ �� �����" << endl;
                    corr = true;

                    file.seekg(entry.index * sizeof(DateOfBirth), ios::beg);
                    if (file.read(reinterpret_cast<char*>(&policy), sizeof(DateOfBirth)))
                    {
                        cout << "   ���� �������� " << policy.number << endl;
                        cout << "   ������� " << policy.surname << endl;
                    }
                }
                file.close();
                return;
            }
            else
                corr = false;
        }
    }
    if (corr == false)
        cout << endl << "���� �������� �� ������ � �����" << endl;
}


//������������� �������
void Rehash(unordered_map<int, list<HashTableEntry>>& hashTable, int newTableSize)
{
    unordered_map<int, list<HashTableEntry>> newHashTable(newTableSize);
    for (const auto& entry : hashTable)//������ �� ���� ��������� ���-�������
    {
        for (const HashTableEntry& hashEntry : entry.second)//������ �� ���� ������� ����
        {
            int newHash = HashFun(hashEntry.key, newTableSize);
            newHashTable[newHash].push_back(hashEntry);
        }
    }
    hashTable = newHashTable;
}


//����������� �������� �����
bool IsPrime(int n)
{
    if (n <= 1)
    {
        return false;
    }
    if (n <= 3)
    {
        return true;
    }
    if (n % 2 == 0 || n % 3 == 0)
    {
        return false;
    }
    for (int i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
        {
            return false;
        }
    }
    return true;
}


int CalculateSize(int currentSize)
{
    //��������� �������� ������� �� ��������� ���������

    int newSize = currentSize * 2;
    //�������� �� ������� �����, ����� ��������� ����������� ��������
    while (!IsPrime(newSize))
    {
        newSize++;
    }
    return newSize;
}


void Output(unordered_map<int, list<HashTableEntry>>& hashTable)
{
    //����� ������ ���-���� � �����
    cout << endl << "����� ���������" << endl;
    for (const auto& entry : hashTable)
    {
        int hash = entry.first;//��������� ���-����
        const list<HashTableEntry>& entries = entry.second;//��������� ������ �������
        for (const HashTableEntry& hashEntry : entries)
        {
            unsigned long int key = hashEntry.key;//��������� �����
            printf("   ������ ������:  %3d   ����:  %8d   ���:  %2d\n", hashEntry.index, key, hash);
        }
    }
}


int main()
{
    setlocale(LC_ALL, "ru");
    srand(static_cast<unsigned int>(time(nullptr)));//������������� ����������
    const string text = "textFile.txt";
    const string binary = "binaryFile.bin";

    ofstream textFile(text);//��������� ����
    if (!textFile)
    {
        cout << "��������� ���� �� ������� ������� ��� ������" << endl;
        return 1;
    }
    cout << "��������� ���� ������� ������ ��� ������" << endl << endl;

    //��������� ���� ��� ������ � �������� ������ 
    ofstream binaryFileOut(binary, ios::binary);//�������� ���� ios::binary -  ������������ ����������� ��������� ���� ��� ���������� �������� �������� �����-������
    if (!binaryFileOut)
    {
        cerr << "�������� ���� �� ������� ������� ��� ������" << endl;
        return 1;
    }
    cout << "�������� ���� ������� ������ ��� ������" << endl << endl;

    int quant = 10;//���������� �������
    set<int> unique;

    DateOfBirth policy;
    for (int i = 0; i < quant; ++i)
    {
        do {
            policy.number = Randomizer();
        } while (unique.count(policy.number) > 0);
        unique.insert(policy.number);
  
        policy.surname = "�������_" + to_string(i);

        textFile << policy.number << ' '  << policy.surname << '\n';

        binaryFileOut.write(reinterpret_cast<const char*>(&policy), sizeof(policy));
    }
    cout << "��������� ���������� � ��������� ����� ���������" << endl;

    size_t bytes = sizeof(DateOfBirth);
    cout << "������ ������ � ������: " << bytes << endl;
    cout << "���������� �������: " << quant << endl << endl;

    textFile.close();
    binaryFileOut.close();

    int tableSize = 11;//������ ���-�������
    unordered_map<int, list<HashTableEntry>> hashTable;//�������� ���-������� � ���������

    //��������� �������� � ���-�������
    ifstream binaryFileIn(binary, ios::binary);
    if (!binaryFileIn)
    {
        cout << "�������� ���� �� ������� ������� ��� ������" << endl;
        return 1;
    }
    cout << "�������� ���� ������� ������ ��� ������" << endl << endl;

    cout << "   �������� ���-��������" << endl << endl;
    int index = 0;
    while (binaryFileIn.read(reinterpret_cast<char*>(&policy), sizeof(policy)))
    {
        //�������� ������ ��� ���-�������
        HashTableEntry entry;
        entry.key = policy.number;
        entry.index = index;
        index++;
        int hash = HashFun(entry.key, tableSize);//���������� ���-���� ��� �����
        hashTable[hash].push_back(entry);//���������� ������ � ������� ���-�������

        printf("   ������ ������:  %3d   ����:  %2d   ���:  %2d\n", entry.index, entry.key, hash);
    }
    binaryFileIn.close();

    //������ ���������� (�������� ��� �������� �����)
    double loadFactor = static_cast<double>(hashTable.size()) / tableSize;
    if (loadFactor > 0.8)
    {
        int newTableSize = CalculateSize(tableSize);//----> ����������� ������ ������� �������
        tableSize = newTableSize;
        Rehash(hashTable, tableSize);
    }

    bool flag = true;
    auto start_time = chrono::high_resolution_clock::now();
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    while (flag)
    {
        int choice;
        cout << endl << "�������� �������� �� ����������: " << endl << "   1 - ������� ����� � �������" << endl << "   2 - ����� ����� � �����" << endl << "   3 - �������� ����� �� ����� � �������" << endl << "   4 - ����� �� ������������" << endl;
        cin >> choice;
        switch (choice)
        {
        case 1:
            unsigned long int insert;
            cout << "������� ���� ��������, ������� �� ������� �������� (8 �����) ";
            cin >> insert;
            Insert(hashTable, insert, tableSize, index);
            loadFactor = static_cast<double>(hashTable.size()) / tableSize;
            if (loadFactor > 0.8)
            {
                int newTableSize = CalculateSize(tableSize);//����������� ������ ������� �������
                tableSize = newTableSize;
                Rehash(hashTable, newTableSize);
            }
            Output(hashTable);
            break;
        case 2:
            unsigned long int found;
            cout << "������� ���� ��������, ������� �� ������� ����� (8 �����) ";
            cin >> found;
            start_time = chrono::high_resolution_clock::now();

            Find(hashTable, policy, binary, found, tableSize);

            end_time = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
            cout << endl << " ����� ����������: " << duration.count() << " [���]" << endl;
            break;
        case 3:
            unsigned long int remove;
            cout << "������� ���� ��������, ������� �� ������� ������� (8 �����) ";
            cin >> remove;
            Remove(hashTable, policy, binary, remove, tableSize);
            Output(hashTable);
            break;
        case 4:
            flag = false;
            break;
        default:
            break;
        }
    }
    return 0;
}