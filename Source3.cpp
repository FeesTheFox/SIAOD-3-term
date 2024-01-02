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
    int offset; //�������� (������ ���������  ����)))
};

unsigned long long RandPhoneNumber() //��������� ����� ��������
{
    long long min = 79850000000;
    long long max = 79999999999;
    return min + rand() % (max - min + 1);
}

string RandSurname() 
{
    string surnames[10] = { "�������", "�������", "�������", "��������", "��������", "�������", "��������", "����", "������������", "�������" };
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
        cout << "�������� ���� �� ������� ������� ��� ������." << endl;
        exit(1);
    }
    cout << "�������� ���� ������� ������ ��� ������." << endl << endl;

    fileT.seekg(offset * sizeof(TaxRecord), ios::beg);

    if (fileT.read(reinterpret_cast<char*>(&record), sizeof(TaxRecord)))
    {
        if (record.INN != 0) {
            cout << "������ ��� �������!" << endl;
            cout << "����� ���: " << record.INN << endl;
            cout << "�������: " << record.surname << endl;
            cout << "����� ��������: +" << record.number << endl;
        }
    }
    else
    {
        cout << endl << "�������� ���� �� ������� ���������" << endl;
    }
    fileT.close();
}

//mid = left + ((key - a.left) *(right - left))/(a.right - a.left)
//���������������� �����
int InterpolationSearch(const vector <Tablet> TabletC, long long key)
{
    int left = 0;  // ����� ������� ������ (����� �������, ��� �������� ������� ���������� � ����) 
    int  right = TabletC.size() - 1; // ������ ������� ������ 


    while (TabletC[left].key < key and key < TabletC[right].key) {
        int mid = left + (key - TabletC[left].key) * (right - left) / (TabletC[right].key - TabletC[left].key);  // ������ ��������, � ������� ����� ��������� ��������� 
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
        return -1; // ���� ������ �������� � ������� ��� 
    }
    
}





int main()
{
    setlocale(LC_ALL, "Rus");
    srand(static_cast<unsigned int>(time(nullptr)));


    ofstream fileText("text.txt");
    if (!fileText)
    {
        cout << "��������� ���� �� ������� ������� ��� ������." << endl;
        return 1;
    }
    cout << "��������� ���� ������� ������ ��� ������." << endl << endl;

    ofstream fileBinaryOUT("text.bin", ios::binary);
    if (!fileBinaryOUT) {
        cout << "�������� ���� �� ������� ������� ��� ������." << endl;
        return 1;
    }
    cout << "�������� ���� ������� ������ ��� ������." << endl << endl;

    int recordcount = 5;
    set<int> unique; //������ ���������� ��������

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

        // ������ � �������� ���� � ���������� ������
        fileBinaryOUT.write(reinterpret_cast<const char*>(&record), sizeof(record));
    }
    cout << "��������� ���������� � ��������� ����� ���������." << endl << endl;




   /*size_t bytes = sizeof(TaxRecord);
   cout << "������ ������ � ������: " << bytes << endl;
   cout << "���������� �������: " << recordcount << endl << endl;*/

    fileText.close();
    fileBinaryOUT.close();



    //���� ������ ���� ������ � �������� ���� ��� ������
    ifstream fileBinaryIN("text.bin", ios::binary);
    if (!fileBinaryIN)
    {
        cout << "�������� ���� �� ������� ������� ��� ������ " << endl;
        return 1;
    }
    cout << "���� ������ ��� ������" << endl;

    //���������� �������
    vector <Tablet> TabletC;
    int offset = 0;
    while (fileBinaryIN.read(reinterpret_cast<char*>(&record), sizeof(record)))
    {
        Tablet container;
        container.key = record.INN; //���������� ������
        container.offset = offset; //������ ������ �� ���� 
        offset++;
        TabletC.push_back(container); //���������� �������� � �������
    }
    fileBinaryIN.close();


    sort(TabletC.begin(), TabletC.end(), [](const Tablet& a, const Tablet& b) {return a.key < b.key; }); //���������� �������

    cout << "!----����� ���----!" << endl << endl;
    long long key;
    cout << "������� ���-����� (10 ����): ";
    cin >> key;


    auto start = chrono::high_resolution_clock::now();

    int result = InterpolationSearch(TabletC, key);
    if (result == -1)
    {
        cout << "������� �� ������\n";
        return 0;
    }

    //�������� �����
    //while (fileBinaryIN.read(reinterpret_cast<char*>(&record), sizeof(record)))
    //{
    //    if (record.INN == key)
    //    {
    //        cout << endl << " ��� ������" << endl;
    //        cout << " ��� " << record.INN << endl;
    //        cout << " ������� " << record.surname << endl;
    //        cout << " ����� �������� +" << record.number << endl;
    //        //�����������: ���� ������, ������� ������ ������� 0 � ������� ����������
    //      
    //        fileBinaryIN.close();
    //        auto end = chrono::high_resolution_clock::now(); 
    //        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    //        cout << " ����� ����������: " << duration.count() << " [���]" << endl;
    //        return 0;
    //    }
    //}
    // :3 //
    //�����������: ���� �� ������, ������� ������ ������� 0 � ������� ���������
    /*cout << "������ � ������� " << key << " �� ���� �������." << endl;
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "����� ���������� ������: " << duration.count() << " ������." << endl;
    fileBinaryIN.close();*/

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
    Read("text.bin", TabletC[result].offset, record);
    cout << "����� ���������� ������: " << duration.count() << " ����������." << endl;

        


    return 0;
}