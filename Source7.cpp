#include <iostream>
#include<queue>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <ctime>
#include <chrono>
using namespace std;

const int t = 2;
int rotationCounter = 0;
struct BNode {
    int keys[2 * t][2];
    BNode* children[2 * t + 1];
    BNode* parent;
    int count; //���������� ������
    int countSons; //���������� �������
    bool leaf;
};
// ��������� ������ � ������������ ����������
struct FlightRecord {
    string departure_point;
    int flight_number;
    string arrival_date;
    string arrival_time;
    double delay_hours;
};

class Tree {
private:
    BNode* root;
    void insert_to_node(int key, int offset, BNode* node);
    void sort(BNode* node);
    void restruct(BNode* node);
    void deletenode(BNode* node);
    int searchKey(int key, BNode* node, int newOffset = -1);
    void remove(int key, BNode* node);
    void removeFromNode(int key, BNode* node);
    void removeLeaf(int key, BNode* node);
    void lconnect(BNode* node, BNode* othernode);
    void rconnect(BNode* node, BNode* othernode);
    void repair(BNode* node);

    void printTreeHierarchy(BNode* root, string indent = "");


public:
    Tree();
    ~Tree();
    void insert(int key, int offset);
    int search(int key, int newOffset = -1);
    void remove(int key);
    int lastDeleted = -1;
    void print();
};

// ������ ������ ������
void Tree::print() {
    if (root != nullptr) {
        cout << "B-Tree structure and keys:" << endl;
        printTreeHierarchy(root, "");
    }
    else {
        cout << "Empty B-Tree" << endl;
    }
}
void Tree::printTreeHierarchy(BNode* root, string indent) {
    if (root != nullptr) {
        cout << indent;
        cout << "| ";
        for (int i = 0; i < root->count; i++) {
            cout << root->keys[i][0] /*<< ", " << root->keys[i][1]*/ << "; ";
        }
        cout << "|" << endl;

        for (int i = 0; i <= 2 * t - 1; i++) {
            printTreeHierarchy(root->children[i], indent + "  ");
        }
    }
}

class FileManager {
public:
    int cnt = 0;
    FileManager() {}
    int lastOffset = 0;
    string filename = "test.bin";
    void createFile(int N);
    bool isUnique(const vector<FlightRecord*>& flightRecords, int key);
    FlightRecord getFlightRecord(int recordOffset, ifstream& fin);

};

bool FileManager::isUnique(const vector<FlightRecord*>& flightRecords, int key) {
    for (const FlightRecord* record : flightRecords) {
        if (record->flight_number == key) {
            return false;
        }
    }
    return true;
}
// ������� ��� �������� ����� � �������� � ������ � ���������� ��� �������
void FileManager::createFile(int N) {
    // ������������� ���������� ��������� �����
    srand(time(NULL));

    ofstream outputFile("text.txt");

    // ������ ��� �������� ������� � ������
    vector<FlightRecord*> records;

    string citis[] = { "City_10", "City_22", "City_40",
        "City_16", "City_12", "City_18", "City_17" };

    // ��������� � ������ ��������� ������� � ������
    for (int i = 0; i < N; i++) {
        FlightRecord* new_record = new FlightRecord;
        int new_key = rand();

        new_record->departure_point = citis[rand() % 7];

        while (!isUnique(records, new_key))
            new_key = rand();


        /*new_key = i;*/
        new_record->flight_number = new_key;

        new_record->arrival_date = "2023-10-0" + to_string(rand() % 10);
        new_record->arrival_time = "10:0" + to_string(i % 10);
        new_record->delay_hours = 1.5;


        outputFile << new_record->departure_point << " "
            << new_record->flight_number << " "
            << new_record->arrival_date << " "
            << new_record->arrival_time << " "
            << new_record->delay_hours << " " << std::endl;
        records.push_back(new_record);
    }

    outputFile.close();

    // �������� ��������� ����� ��� ������
    ifstream inputFile("text.txt");
    ofstream fout("test.bin", ios::binary);
    FlightRecord record;
    int i = 0;

    // ������ ������ � ������ � ����
    while (inputFile >> record.departure_point >> record.flight_number >>
        record.arrival_date >> record.arrival_time >> record.delay_hours) {
        if (i == N - 1 or i == N / 2 or i == 0)
            cout << "��� ��� ��������: " << records[i]->flight_number << endl;

        //����� ������
        const char* departure = records[i]->departure_point.c_str();
        int departureLength = records[i]->departure_point.size();
        fout.write(departure, departureLength + 1);

        fout.write((char*)&records[i]->flight_number, sizeof(int));

        //���� �������
        const char* date = records[i]->arrival_date.c_str();
        int dateLength = records[i]->arrival_date.size();
        fout.write(date, dateLength + 1);

        //����� �������
        const char* time = records[i]->arrival_time.c_str();
        int timeLength = records[i]->arrival_time.size();
        fout.write(time, timeLength + 1);

        fout.write((char*)&records[i]->delay_hours, sizeof(double));
        cnt++;
        i++;
    }
    // �������� �����
    fout.close();
    inputFile.close();
}

FlightRecord FileManager::getFlightRecord(int recordOffset, ifstream& fin) {
    FlightRecord tmp; // ������� ������ ��������� Record ��� �������� ������.

    fin.seekg(recordOffset); // ������������� ������� ������ � ����� �� ��������� ��������.

    // ������ ������ �� �����

    string departure_point; // ������� ������ ��� �������� ������ � ��������.
    char ch;
    fin.get(ch);
    while (ch != '\0') {
        departure_point += ch; // ��������� ������� �� ������� '\0' (�������� �������) � ��������� �� � ������ companyData.
        fin.get(ch);
    }
    tmp.departure_point = departure_point; // ����������� ���������� ������ ���� company ������� tmp.
    fin.read((char*)&tmp.flight_number, sizeof(int)); // ��������� ����� ����� (int) � ��������� ��� � ���� flight_number.

    string arrival_date; // ������� ������ ��� �������� ������ � �������.
    fin.get(ch);
    while (ch != '\0') {
        arrival_date += ch; // ��������� ������� �� ������� '\0' (�������� �������) � ��������� �� � ������ nameData.
        fin.get(ch);
    }
    tmp.arrival_date = arrival_date; // ����������� ���������� ������ ���� arrival_date ������� tmp.

    string arrival_time; // ������� ������ ��� �������� ������ � �������.
    fin.get(ch);
    while (ch != '\0') {
        arrival_time += ch; // ��������� ������� �� ������� '\0' (�������� �������) � ��������� �� � ������ nameData.
        fin.get(ch);
    }
    tmp.arrival_time = arrival_time; // ����������� ���������� ������ ���� arrival_time ������� tmp.
    fin.read((char*)&tmp.delay_hours, sizeof(double));

    lastOffset = fin.tellg();
    return tmp; // ���������� ������ tmp, ���������� ��������� ������.
}
// �����������
Tree::Tree() { root = nullptr; }

// ����� ��� ������� ����� � �������� � ����
void Tree::insert_to_node(int key, int offset, BNode* node) {
    // ��������� ���� � �������� � ����
    node->keys[node->count][0] = key;
    node->keys[node->count][1] = offset;
    node->count = node->count + 1;
    // ��������� ����� � ����
    sort(node);
}
// ���������� ��������� � �������� ���������
void Tree::sort(BNode* node) {
    int m, n;
    for (int i = 0; i < (2 * t - 1); i++) {
        bool flag = true;
        for (int j = 0; j <= (2 * t - 1) - i; j++) {
            if ((node->keys[j][0] > 0) && (node->keys[j + 1][0] > 0)) {
                if ((node->keys[j][0]) > (node->keys[j + 1][0])) {
                    m = node->keys[j][0];
                    n = node->keys[j][1];
                    node->keys[j][0] = node->keys[j + 1][0];
                    node->keys[j][1] = node->keys[j + 1][1];
                    node->keys[j + 1][0] = m;
                    node->keys[j + 1][1] = n;
                    flag = false;
                }
            }
            else break;
        }
        if (flag)
            break;
    }
}

// ����� ��� ������� ����� � �������� � ����
void Tree::insert(int key, int offset) {
    if (root == nullptr) {
        // ���� ������ �����, ������� ����� �������� ����
        BNode* newRoot = new BNode;
        newRoot->keys[0][0] = key;
        newRoot->keys[0][1] = offset;
        for (int j = 1; j <= (2 * t - 1); j++) {
            newRoot->keys[j][0] = 0;
            newRoot->keys[j][1] = -1;
        }
        for (int i = 0; i <= (2 * t); i++)
            newRoot->children[i] = nullptr;
        newRoot->count = 1;
        newRoot->countSons = 0;
        newRoot->leaf = true;
        newRoot->parent = nullptr;
        root = newRoot;
    }
    else {
        BNode* ptr = root;
        // ������� ����, � ������� ����� �������� ���� � ��������
        while (ptr->leaf == false) {
            for (int i = 0; i <= (2 * t - 1); i++) {
                if (ptr->keys[i] != 0) {
                    if (key < ptr->keys[i][0]) {
                        ptr = ptr->children[i];
                        break;
                    }
                    if ((ptr->keys[i + 1][0] == 0) && (key > ptr->keys[i][0])) {
                        ptr = ptr->children[i + 1];
                        break;
                    }
                }
                else break;
            }
        }
        // ��������� ���� � �������� � ��������� ����
        insert_to_node(key, offset, ptr);
        // ��������������� ������ ��� �������������
        while (ptr->count == 2 * t) {
            if (ptr == root) {
                restruct(ptr);
                break;
            }
            else {
                restruct(ptr);
                ptr = ptr->parent;
            }
        }
    }
}

// ����� ��� ���������������� ����
void Tree::restruct(BNode* node) {
    if (node->count < (2 * t - 1)) return;  // ���� ���� �� ����������, �������

    // ������� ���� ����� �������
    BNode* child1 = new BNode;
    BNode* child2 = new BNode;


    rotationCounter++;
    // ��������� ������� ����
    for (int j = 0; j <= t - 2; j++) {
        child1->keys[j][0] = node->keys[j][0];
        child1->keys[j][1] = node->keys[j][1];
    }
    for (int j = t - 1; j <= (2 * t - 1); j++) {
        child1->keys[j][0] = 0;
        child1->keys[j][1] = -1;
    }
    child1->count = t - 1; // ���������� ������ � ����

    // ��������� ������� ����
    for (int j = 0; j <= (t - 1); j++) {
        child2->keys[j][0] = node->keys[j + t][0];
        child2->keys[j][1] = node->keys[j + t][1];
    }
    for (int j = t; j <= (2 * t - 1); j++) {
        child2->keys[j][0] = 0;
        child2->keys[j][1] = -1;
    }
    child2->count = t; // ���������� ������ � ����

    // ��������� �������, ������� ���������� ������ �� ��������
    if (node->countSons != 0) {
        for (int i = 0; i <= (t - 1); i++) {
            child1->children[i] = node->children[i];
            child1->children[i]->parent = child1;
        }
        for (int i = t; i <= (2 * t); i++) child1->children[i] = nullptr;
        child1->leaf = false;
        child1->countSons = t - 1; // ���������� �������
    }
    else {
        child1->leaf = true;
        child1->countSons = 0;
        for (int i = 0; i <= (2 * t); i++) child1->children[i] = nullptr;
    }

    if (node->countSons != 0) {
        for (int i = 0; i <= (t); i++) {
            child2->children[i] = node->children[i + t];
            child2->children[i]->parent = child2;
        }
        for (int i = t + 1; i <= (2 * t); i++) child2->children[i] = nullptr;
        child2->leaf = false;
        child2->countSons = t; // ���������� �������
    }
    else {
        child2->leaf = true;
        child2->countSons = 0;
        for (int i = 0; i <= (2 * t); i++) child2->children[i] = nullptr;
    }

    // ������������ ����
    if (node->parent == nullptr) { // ���� �������� ���, �� ��� ������
        BNode* newRoot = new BNode;
        newRoot->keys[0][0] = node->keys[t - 1][0];
        newRoot->keys[0][1] = node->keys[t - 1][1];
        for (int j = 1; j <= (2 * t - 1); j++) {
            newRoot->keys[j][0] = 0;
            newRoot->keys[j][1] = -1;
        }
        newRoot->children[0] = child1;
        newRoot->children[1] = child2;
        for (int i = 2; i <= (2 * t); i++) newRoot->children[i] = nullptr;
        newRoot->parent = nullptr;
        newRoot->leaf = false;
        newRoot->count = 1;
        newRoot->countSons = 2 * t - 2;
        child1->parent = newRoot;
        child2->parent = newRoot;
        root = newRoot;  // ��������� ������
    }
    else {
        // ��������� ���� � ������� � ������������ ����
        int keyToInsert = node->keys[t - 1][0];
        int offsetToInsert = node->keys[t - 1][1];
        BNode* parent = node->parent;
        insert_to_node(keyToInsert, offsetToInsert, parent);

        // ��������� ������ �� ������� � ������������ ����
        for (int i = 0; i <= (2 * t); i++) {
            if (parent->children[i] == node) parent->children[i] = nullptr;
        }
        for (int i = 0; i <= (2 * t); i++) {
            if (parent->children[i] == nullptr) {
                for (int j = (2 * t); j > (i + 1); j--) parent->children[j] = parent->children[j - 1];
                parent->children[i + 1] = child2;
                parent->children[i] = child1;
                break;
            }
        }
        child1->parent = parent;
        child2->parent = parent;
        parent->leaf = false;
    }
}

// ����� ��� ������ ����� � ������ �� ������� �������� ���������
int Tree::search(int key, int newOffset) {
    return searchKey(key, this->root, newOffset);
}

int cnt = 0;
// ����� ��� ������ ����� � ������ �, ��� �������������, ���������� offset
int Tree::searchKey(int key, BNode* node, int newOffset) {
    cnt++;
    if (node != nullptr) {
        cnt++;
        if (!node->leaf) {
            int i;
            // ����� ������ ���� � ����������� ������
            for (i = 0; i < (2 * t - 1); i++) {
                cnt += 2;
                if (node->keys[i][0] != 0) {
                    cnt++;
                    if (key == node->keys[i][0]) {
                        if (newOffset == -1) {
                            return node->keys[i][1];  // ���������� ������ ������� ���������� �����

                        }
                        else {
                            node->keys[i][1] = newOffset;
                            return node->keys[i][1];
                        }
                    }
                    cnt++;
                    if (key < node->keys[i][0]) {
                        return searchKey(key, node->children[i], newOffset);
                    }
                }
                else {
                    break;
                }
            }
            return searchKey(key, node->children[i], newOffset);
        }
        else {
            // ����� ������ ��������� ����
            for (int j = 0; j < (2 * t - 1); j++) {
                cnt += 2;
                if (key == node->keys[j][0]) {
                    if (newOffset == -1) {

                        return node->keys[j][1];  // ���������� ������ ������� ���������� �����
                    }
                    else {
                        node->keys[j][1] = newOffset;
                        return node->keys[j][1];
                    }
                }
            }
            return -1;  // ���������� -1, ���� ���� �� ������
        }
    }
    else {
        return -1;  // ���������� -1, ���� ���� ����� nullptr
    }
}

// ����� ��� �������� ����� �� ���� node
void Tree::removeFromNode(int key, BNode* node) {
    for (int i = 0; i < node->count; i++) {
        if (node->keys[i][0] == key) {
            // �������� ����� � �������� ����� �� ���� �������, ������� � ��������� �������
            for (int j = i; j < node->count; j++) {
                node->keys[j][0] = node->keys[j + 1][0];
                node->keys[j][1] = node->keys[j + 1][1];
                node->children[j] = node->children[j + 1];
            }
            // �������� ��������� ���� � �������
            node->keys[node->count - 1][0] = 0;
            node->keys[node->count - 1][1] = -1;
            node->children[node->count - 1] = node->children[node->count];
            node->children[node->count] = nullptr;
            // ��������� ���������� ������ � ����
            node->count--;
            break;
        }
    }
}

// ������� ���� ����� � ������ � ����� ��������������
// ����� ��� ����������� ���� �����: node � othernode
void Tree::lconnect(BNode* node, BNode* othernode) {
    if (node == nullptr) return;
    // ��������� ����� � �������� �� othernode � node
    for (int i = 0; i <= (othernode->count - 1); i++) {
        node->keys[node->count][0] = othernode->keys[i][0];
        node->keys[node->count][1] = othernode->keys[i][1];
        node->children[node->count] = othernode->children[i];
        node->count = node->count + 1;
    }
    // ��������� ���������� ������� �� othernode � node
    node->children[node->count] = othernode->children[othernode->count];
    // ��������� parent ��� �������� � node
    for (int j = 0; j <= node->count; j++) {
        if (node->children[j] == nullptr) break;
        node->children[j]->parent = node;
    }
    // ������� othernode
    delete othernode;
}

// ����� ��� ����������� ���� �����: node � othernode
void Tree::rconnect(BNode* node, BNode* othernode) {
    if (node == nullptr) return;

    // ��������� ����� � �������� �� othernode � node
    for (int i = 0; i <= (othernode->count - 1); i++) {
        node->keys[node->count][0] = othernode->keys[i][0];
        node->keys[node->count][1] = othernode->keys[i][1];
        node->children[node->count + 1] = othernode->children[i + 1];
        node->count = node->count + 1;
    }
    // ��������� parent ��� �������� � node
    for (int j = 0; j <= node->count; j++) {
        if (node->children[j] == nullptr) break;
        node->children[j]->parent = node;
    }
    // ������� othernode
    delete othernode;
}

// ����� ������� ������ ����� �������� �����
void Tree::repair(BNode* node) {
    // ���� ���� �������� ������ � ����
    if ((node == root) && (node->count == 0)) {
        // ���� � ����� ���� �������, ������ ��� ����� ������
        if (root->children[0] != nullptr) {
            root->children[0]->parent = nullptr;
            root = root->children[0];
        }
        else {
            delete root;
        }
        return;
    }

    BNode* ptr = node;
    int positionSon;
    BNode* parent = ptr->parent;

    // ������� ������� ���� �� ��������� � ��������
    for (int j = 0; j <= parent->count; j++) {
        if (parent->children[j] == ptr) {
            positionSon = j;
            break;
        }
    }

    // ���� ptr - ������ ������� (����� �����)
    if (positionSon == 0) {
        insert_to_node(parent->keys[positionSon][0], parent->keys[positionSon][1], ptr);
        lconnect(ptr, parent->children[positionSon + 1]);
        parent->children[positionSon + 1] = ptr;
        parent->children[positionSon] = nullptr;
        removeFromNode(parent->keys[positionSon][0], parent);

        // ���� ����� ������� � ptr ������� ����� ������, �������� ����������������
        if (ptr->count == 2 * t) {
            while (ptr->count == 2 * t) {
                if (ptr == root) {
                    restruct(ptr);
                    break;
                }
                else {
                    restruct(ptr);
                    ptr = ptr->parent;
                }
            }
        }
        // �����, ���� � �������� ������������ ������, ���������� ������
        else if (parent->count <= (t - 2)) {
            repair(parent);
        }
    }
    else {
        // ���� ptr - ��������� ������� (����� ������)
        if (positionSon == parent->count) {
            insert_to_node(parent->keys[positionSon - 1][0], parent->keys[positionSon - 1][1], parent->children[positionSon - 1]);
            lconnect(parent->children[positionSon - 1], ptr);
            parent->children[positionSon] = parent->children[positionSon - 1];
            parent->children[positionSon - 1] = nullptr;
            removeFromNode(parent->keys[positionSon - 1][0], parent);
            BNode* temp = parent->children[positionSon];

            // ���� ����� ������� � temp ������� ����� ������, �������� ����������������
            if (ptr->count == 2 * t) {
                while (temp->count == 2 * t) {
                    if (temp == root) {
                        restruct(temp);
                        break;
                    }
                    else {
                        restruct(temp);
                        temp = temp->parent;
                    }
                }
            }
            // �����, ���� � �������� ������������ ������, ���������� ������
            else if (parent->count <= (t - 2)) {
                repair(parent);
            }
        }
        else { // ���� ptr ����� ������� ������ � �����
            insert_to_node(parent->keys[positionSon][0], parent->keys[positionSon][1], ptr);
            lconnect(ptr, parent->children[positionSon + 1]);
            parent->children[positionSon + 1] = ptr;
            parent->children[positionSon] = nullptr;
            removeFromNode(parent->keys[positionSon][0], parent);

            // ���� ����� ������� � ptr ������� ����� ������, �������� ����������������
            if (ptr->count == 2 * t) {
                while (ptr->count == 2 * t) {
                    if (ptr == root) {
                        restruct(ptr);
                        break;
                    }
                    else {
                        restruct(ptr);
                        ptr = ptr->parent;
                    }
                }
            }
            // �����, ���� � �������� ������������ ������, ���������� ������
            else if (parent->count <= (t - 2)) {
                repair(parent);
            }
        }
    }
}

// ����� �������� �� �������
void Tree::removeLeaf(int key, BNode* node) {
    if ((node == root) && (node->count == 1)) {
        removeFromNode(key, node);
        root->children[0] = nullptr;
        delete root;
        root = nullptr;
        return;
    }
    if (node == root) {
        removeFromNode(key, node);
        return;
    }
    BNode* ptr = node;
    int k1;
    int k2;
    int positionSon;
    BNode* parent = ptr->parent;
    for (int j = 0; j <= parent->count; j++) {
        if (parent->children[j] == ptr) {
            positionSon = j; //������� ���� �� ��������� � ��������
            break;
        }
    }
    //���� ptr-������ ������� (����� �����)
    if (positionSon == 0) {
        if (parent->children[positionSon + 1]->count > (t - 1)) { //���� � ������� ����� ������, ��� t-1 ������
            k1 = parent->children[positionSon + 1]->keys[0][0]; //k1 - ����������� ���� ������� �����
            k2 = parent->keys[positionSon][0]; //k2 - ���� ��������, ������, ��� ���������, � ������, ��� k1
            int k2Offset = parent->keys[positionSon][1];
            insert_to_node(k2, k2Offset, ptr);
            removeFromNode(key, ptr);
            parent->keys[positionSon][0] = k1; //������ ������� k1 � k2
            removeFromNode(k1, parent->children[positionSon + 1]); //������� k1 �� ������� �����
        }
        else { //���� � ������� ������������� ����� �� ������ t-1 ������		
            removeFromNode(key, ptr);
            if (ptr->count <= (t - 2)) repair(ptr);
        }
    }
    else {
        //���� ptr-��������� ������� (����� ������)
        if (positionSon == parent->count) {
            //���� � ������ ����� ������, ��� t-1 ������
            if (parent->children[positionSon - 1]->count > (t - 1)) {
                BNode* temp = parent->children[positionSon - 1];
                k1 = temp->keys[temp->count - 1][0]; //k1 - ������������ ���� ������ �����
                k2 = parent->keys[positionSon - 1][0]; //k2 - ���� ��������, ������, ��� ��������� � ������, ��� k1
                int k2Offset = parent->keys[positionSon - 1][1];
                insert_to_node(k2, k2Offset, ptr);
                removeFromNode(key, ptr);
                parent->keys[positionSon - 1][0] = k1;
                removeFromNode(k1, temp);
            }
            else { //���� � ������������� ������ ����� �� ������ t-1 ������
                removeFromNode(key, ptr);
                if (ptr->count <= (t - 2)) repair(ptr);
            }
        }
        else { //���� ptr ����� ������� ������ � �����
            //���� � ������� ����� ������, ��� t-1 ������
            if (parent->children[positionSon + 1]->count > (t - 1)) {
                k1 = parent->children[positionSon + 1]->keys[0][0]; //k1 - ����������� ���� ������� �����
                k2 = parent->keys[positionSon][0]; //k2 - ���� ��������, ������, ��� ��������� � ������, ��� k1
                int k2Offset = parent->keys[positionSon][1];
                insert_to_node(k2, k2Offset, ptr);
                removeFromNode(key, ptr);
                parent->keys[positionSon][0] = k1; //������ ������� k1 � k2
                removeFromNode(k1, parent->children[positionSon + 1]); //������� k1 �� ������� �����
            }
            else {
                //���� � ������ ����� ������, ��� t-1 ������
                if (parent->children[positionSon - 1]->count > (t - 1)) {
                    BNode* temp = parent->children[positionSon - 1];
                    k1 = temp->keys[temp->count - 1][0]; //k1 - ������������ ���� ������ �����
                    k2 = parent->keys[positionSon - 1][0]; //k2 - ���� ��������, ������, ��� ��������� � ������, ��� k1
                    int k2Offset = parent->keys[positionSon - 1][1];
                    insert_to_node(k2, k2Offset, ptr);
                    removeFromNode(key, ptr);
                    parent->keys[positionSon - 1][0] = k1;
                    removeFromNode(k1, temp);
                }
                else { //���� � ����� ������� �� ������ t-1 ������
                    removeFromNode(key, ptr);
                    if (ptr->count <= (t - 2)) repair(ptr);
                }
            }
        }
    }
}

// ����� �������� ��� ���� ����� ����� ��������
void Tree::remove(int key, BNode* node) {
    BNode* ptr = node;
    int position; //����� �����
    int i;
    for (int i = 0; i <= node->count - 1; i++) {
        if (key == node->keys[i][0]) {
            position = i;
            break;
        }
    }
    int positionSon; //����� ���� �� ��������� � ��������
    if (ptr->parent != nullptr) {
        for (int i = 0; i <= ptr->parent->count; i++) {
            if (ptr->children[i] == ptr) {
                positionSon == i;
                break;
            }
        }
    }
    //������� ���������� ���� ������� ���������
    ptr = ptr->children[position + 1];
    int newkey = ptr->keys[0][0];
    while (ptr->leaf == false) ptr = ptr->children[0];
    //���� ������ � ��������� ����� �� ������ 1 - ���� ���������� ���� � ����� ���������
    //����� - ������ �������� key �� ����� ����, ������� ����� ���� �� �����
    if (ptr->count > (t - 1)) {
        newkey = ptr->keys[0][0];
        removeFromNode(newkey, ptr);
        node->keys[position][0] = newkey;
    }
    else {
        ptr = node;
        //���� ���������� ���� � ����� ���������
        ptr = ptr->children[position];
        newkey = ptr->keys[ptr->count - 1][0];
        while (ptr->leaf == false) ptr = ptr->children[ptr->count];
        newkey = ptr->keys[ptr->count - 1][0];
        node->keys[position][0] = newkey;
        if (ptr->count > (t - 1)) removeFromNode(newkey, ptr);
        else {
            //���� ������ �� ������, ��� t-1 - �������������
            removeLeaf(newkey, ptr);
        }
    }
}

// �������� ����� ��������, �� �������� ���������� ��� ������ ��������
void Tree::remove(int key) {
    BNode* ptr = this->root;  // ��������� �� ������ ������
    int position;  // ������� ����� � ����
    int positionSon;  // ������� ������� ����
    int i;

    // ���� ���� �� ������, ��������� �������� ��������
    if (searchKey(key, ptr) == -1) {
        return;
    }
    else {
        // ���� ����, � ������� ��������� ���� ��� ��������
        for (i = 0; i <= ptr->count - 1; i++) {
            if (ptr->keys[i][0] != 0) {
                // ���� ���� ������, ��������� ��� ������� � ������� �� �����
                if (key == ptr->keys[i][0]) {
                    position = i;
                    break;
                }
                else {
                    // ���� ���� ������ ��������, ��������� � ������ �������
                    if ((key < ptr->keys[i][0])) {
                        ptr = ptr->children[i];
                        positionSon = i;
                        i = -1;
                    }
                    else {
                        // ���� �������� ����� ����, ��������� � ������� �������
                        if (i == (ptr->count - 1)) {
                            ptr = ptr->children[i + 1];
                            positionSon = i + 1;
                            i = -1;
                        }
                    }
                }
            }
            else break;
        }
    }
    // ���� ���� �������� ������
    if (ptr->leaf == true) {
        // ���� � ���� ���������� ������, �������� ����� removeFromNode
        if (ptr->count > (t - 1)) {
            removeFromNode(key, ptr);
        }
        // ����� �������� ����� removeLeaf
        else {
            removeLeaf(key, ptr);
        }
    }
    // ���� ���� �� �������� ������, �������� ����������� ����� remove
    else {
        remove(key, ptr);
    }
}

// ����� ��������� ��� ������ �� ����� � B-������
void insertAll(FileManager fm, Tree* tree) {
    ifstream fin(fm.filename, ios_base::binary);  // ��������� ���� ��� ������ � �������� ������
    // �������� �� ���� ������� � �����
    for (int i = 0; i < fm.cnt; i++) {
        int offSet = fm.lastOffset;  // �������� ������� ��������
        // �������� ������ �� ����� �� �������� ��������
        FlightRecord record = fm.getFlightRecord(offSet, fin);
        // ��������� ������ � ������
        tree->insert(record.flight_number, offSet);
    }
    fin.close();  // ��������� ���� ����� �������������
}

void displayFindNum(FileManager fm, Tree* tree) {
    // ������������ ������ ����� ��� ������
    cout << "������� ����� ��� ������: ";
    int num;
    cin >> num;

    // ��������� ���� ��� ������ � �������� ������
    ifstream fin("test.bin", ios::binary);
    /*auto start_time = chrono::high_resolution_clock::now();*/
    // ���� ����� � ������
    int offset = tree->search(num);
    /* auto end_time = chrono::high_resolution_clock::now();
     auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
     cout << "����� ���������� ������� findInFile: " << duration.count() << " ��" << endl;*/
     // ���� ����� �� ������, ������� ��������� � ��������� �������
    if (offset == -1) {
        cout << "����� �� ������." << endl;
        return;
    }
    // �������� ������ � ������ �� �������� � ������� �� �� �����
    FlightRecord record = fm.getFlightRecord(offset, fin);
    cout << record.departure_point << " " << record.flight_number << " " << record.arrival_date <<
        " " << record.arrival_time << " " << record.delay_hours << endl;
    // ��������� ����
    fin.close();

}

void deleteNode(FileManager& fm, Tree* tree) {
    ifstream fin("test.bin", ios::binary);
    cout << "������� ����� ��� ��������: ";
    int data;
    cin >> data;

    int skip_offset = tree->search(data);
    if (skip_offset >= 0)
        skip_offset /= 37;
    tree->remove(data);


    string filename = "test.bin"; 
    string tempFilename = "test_temp.bin"; 

    ofstream tempFile(tempFilename, ios::binary);

    if (!fin || !tempFile) {
        cerr << "������ �������� ������." << endl;
    }
    for (int i = 0; i < fm.cnt; i++)
    {
        if (i == skip_offset) {
            continue;
        }
        FlightRecord tmp; // ������� ������ ��������� Record ��� �������� ������.

        fm.lastOffset = tempFile.tellp();

        fin.seekg(i * 37); // ������������� ������� ������ � ����� �� ��������� ��������.

        string str = ""; // ������� ������ ��� �������� ������ � �������� ����������.
        char ch;
        fin.get(ch);
        while (ch != '\0') {
            str += ch; // ��������� ������� �� ������� '\0' (�������� �������) � ��������� �� � ������ namecinemaa.
            fin.get(ch);
        }
        tmp.departure_point = str; // ����������� ���������� ������ ���� cname ������� tmp.
        // ������ ������ �� �����
        fin.read((char*)&tmp.flight_number, sizeof(int)); // ��������� ����� ����� (int) � ��������� ��� � ���� uncode.

        str = ""; // ������� ������ ��� �������� ������ � �������� ������.
        fin.get(ch);
        while (ch != '\0') {
            str += ch; // ��������� ������� �� ������� '\0' (�������� �������) � ��������� �� � ������ filmnamee.
            fin.get(ch);
        }
        tmp.arrival_date = str; // ����������� ���������� ������ ���� fname ������� tmp.

        str = ""; // ������� ������ ��� �������� ������ � �������� ������.
        fin.get(ch);
        while (ch != '\0') {
            str += ch; // ��������� ������� �� ������� '\0' (�������� �������) � ��������� �� � ������ filmnamee.
            fin.get(ch);
        }
        tmp.arrival_time = str; // ����������� ���������� ������ ���� fname ������� tmp.

        fin.read((char*)&tmp.delay_hours, sizeof(double));

        //�������� ����� ������
        const char* cinemaname = tmp.departure_point.c_str();
        int cinemasize = tmp.departure_point.size();
        tempFile.write(cinemaname, cinemasize + 1);

        tempFile.write((char*)&tmp.flight_number, sizeof(int));

        //�������� ������
        const char* filmname = tmp.arrival_date.c_str();
        int filmsize = tmp.arrival_date.size();
        tempFile.write(filmname, filmsize + 1);

        //���� ������ ������
        const char* date = tmp.arrival_time.c_str();
        int datelength = tmp.arrival_time.size();
        tempFile.write(date, datelength + 1);

        tempFile.write((char*)&tmp.delay_hours, sizeof(double));

        tree->search(tmp.flight_number, fm.lastOffset);
    }

    // ������� �����
    fin.close();
    tempFile.close();
    if (remove(filename.c_str()) != 0) {
        cerr << "������ ��� �������� ��������� �����." << endl;
    }
    // �������� �������� ���� ���������
    if (rename(tempFilename.c_str(), filename.c_str()) != 0) {
        cerr << "������ ��� �������������� ������." << endl;
    }
    cout << "������ ������� �������." << endl;
    fm.cnt--;
}

void main() {
    Tree* tree = new Tree();
    setlocale(LC_ALL, "ru");
    int N = 50;

    FileManager fm;
    fm.createFile(N);
    insertAll(fm, tree);

    tree->print();
    cout << endl;

    displayFindNum(fm, tree);

    deleteNode(fm, tree);
    tree->print();

}