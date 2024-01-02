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
    int count; //количество ключей
    int countSons; //количество сыновей
    bool leaf;
};
// Структура записи о пассажирском авиарейсов
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

// методы вывода дерева
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
// Функция для создания файла с записями о рейсах и заполнения его данными
void FileManager::createFile(int N) {
    // Инициализация генератора случайных чисел
    srand(time(NULL));

    ofstream outputFile("text.txt");

    // Вектор для хранения записей о рейсах
    vector<FlightRecord*> records;

    string citis[] = { "City_10", "City_22", "City_40",
        "City_16", "City_12", "City_18", "City_17" };

    // Генерация и запись случайных записей о рейсах
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

    // Открытие бинарного файла для записи
    ifstream inputFile("text.txt");
    ofstream fout("test.bin", ios::binary);
    FlightRecord record;
    int i = 0;

    // Запись данных о рейсах в файл
    while (inputFile >> record.departure_point >> record.flight_number >>
        record.arrival_date >> record.arrival_time >> record.delay_hours) {
        if (i == N - 1 or i == N / 2 or i == 0)
            cout << "Код для проверки: " << records[i]->flight_number << endl;

        //пункт вылета
        const char* departure = records[i]->departure_point.c_str();
        int departureLength = records[i]->departure_point.size();
        fout.write(departure, departureLength + 1);

        fout.write((char*)&records[i]->flight_number, sizeof(int));

        //Дата прилета
        const char* date = records[i]->arrival_date.c_str();
        int dateLength = records[i]->arrival_date.size();
        fout.write(date, dateLength + 1);

        //время прилета
        const char* time = records[i]->arrival_time.c_str();
        int timeLength = records[i]->arrival_time.size();
        fout.write(time, timeLength + 1);

        fout.write((char*)&records[i]->delay_hours, sizeof(double));
        cnt++;
        i++;
    }
    // Закрытие файла
    fout.close();
    inputFile.close();
}

FlightRecord FileManager::getFlightRecord(int recordOffset, ifstream& fin) {
    FlightRecord tmp; // Создаем объект структуры Record для хранения данных.

    fin.seekg(recordOffset); // Устанавливаем позицию чтения в файле на указанное смещение.

    // Чтение данных из файла

    string departure_point; // Создаем строку для хранения данных о компании.
    char ch;
    fin.get(ch);
    while (ch != '\0') {
        departure_point += ch; // Считываем символы до символа '\0' (нулевого символа) и добавляем их к строке companyData.
        fin.get(ch);
    }
    tmp.departure_point = departure_point; // Присваиваем полученную строку полю company объекта tmp.
    fin.read((char*)&tmp.flight_number, sizeof(int)); // Считываем целое число (int) и сохраняем его в поле flight_number.

    string arrival_date; // Создаем строку для хранения данных о фамилии.
    fin.get(ch);
    while (ch != '\0') {
        arrival_date += ch; // Считываем символы до символа '\0' (нулевого символа) и добавляем их к строке nameData.
        fin.get(ch);
    }
    tmp.arrival_date = arrival_date; // Присваиваем полученную строку полю arrival_date объекта tmp.

    string arrival_time; // Создаем строку для хранения данных о фамилии.
    fin.get(ch);
    while (ch != '\0') {
        arrival_time += ch; // Считываем символы до символа '\0' (нулевого символа) и добавляем их к строке nameData.
        fin.get(ch);
    }
    tmp.arrival_time = arrival_time; // Присваиваем полученную строку полю arrival_time объекта tmp.
    fin.read((char*)&tmp.delay_hours, sizeof(double));

    lastOffset = fin.tellg();
    return tmp; // Возвращаем объект tmp, содержащий считанные данные.
}
// конструктор
Tree::Tree() { root = nullptr; }

// Метод для вставки ключа и смещения в узел
void Tree::insert_to_node(int key, int offset, BNode* node) {
    // Добавляем ключ и смещение в узел
    node->keys[node->count][0] = key;
    node->keys[node->count][1] = offset;
    node->count = node->count + 1;
    // Сортируем ключи в узле
    sort(node);
}
// сортировка пузырьком с условием айверсона
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

// Метод для вставки ключа и смещения в узел
void Tree::insert(int key, int offset) {
    if (root == nullptr) {
        // Если дерево пусто, создаем новый корневой узел
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
        // Находим лист, в который нужно вставить ключ и смещение
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
        // Вставляем ключ и смещение в найденный лист
        insert_to_node(key, offset, ptr);
        // Перебалансируем дерево при необходимости
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

// Метод для перебалансировки узла
void Tree::restruct(BNode* node) {
    if (node->count < (2 * t - 1)) return;  // Если узел не переполнен, выходим

    // Создаем двух новых сыновей
    BNode* child1 = new BNode;
    BNode* child2 = new BNode;


    rotationCounter++;
    // Заполняем первого сына
    for (int j = 0; j <= t - 2; j++) {
        child1->keys[j][0] = node->keys[j][0];
        child1->keys[j][1] = node->keys[j][1];
    }
    for (int j = t - 1; j <= (2 * t - 1); j++) {
        child1->keys[j][0] = 0;
        child1->keys[j][1] = -1;
    }
    child1->count = t - 1; // Количество ключей в узле

    // Заполняем второго сына
    for (int j = 0; j <= (t - 1); j++) {
        child2->keys[j][0] = node->keys[j + t][0];
        child2->keys[j][1] = node->keys[j + t][1];
    }
    for (int j = t; j <= (2 * t - 1); j++) {
        child2->keys[j][0] = 0;
        child2->keys[j][1] = -1;
    }
    child2->count = t; // Количество ключей в узле

    // Настройка сыновей, включая обновление ссылок на родителя
    if (node->countSons != 0) {
        for (int i = 0; i <= (t - 1); i++) {
            child1->children[i] = node->children[i];
            child1->children[i]->parent = child1;
        }
        for (int i = t; i <= (2 * t); i++) child1->children[i] = nullptr;
        child1->leaf = false;
        child1->countSons = t - 1; // Количество сыновей
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
        child2->countSons = t; // Количество сыновей
    }
    else {
        child2->leaf = true;
        child2->countSons = 0;
        for (int i = 0; i <= (2 * t); i++) child2->children[i] = nullptr;
    }

    // Родительский узел
    if (node->parent == nullptr) { // Если родителя нет, то это корень
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
        root = newRoot;  // Обновляем корень
    }
    else {
        // Вставляем ключ и сыновей в родительский узел
        int keyToInsert = node->keys[t - 1][0];
        int offsetToInsert = node->keys[t - 1][1];
        BNode* parent = node->parent;
        insert_to_node(keyToInsert, offsetToInsert, parent);

        // Обновляем ссылки на сыновей в родительском узле
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

// Метод для поиска ключа в дереве по обертке входного параметра
int Tree::search(int key, int newOffset) {
    return searchKey(key, this->root, newOffset);
}

int cnt = 0;
// Метод для поиска ключа в дереве и, при необходимости, обновления offset
int Tree::searchKey(int key, BNode* node, int newOffset) {
    cnt++;
    if (node != nullptr) {
        cnt++;
        if (!node->leaf) {
            int i;
            // Поиск внутри узла с нелистовыми узлами
            for (i = 0; i < (2 * t - 1); i++) {
                cnt += 2;
                if (node->keys[i][0] != 0) {
                    cnt++;
                    if (key == node->keys[i][0]) {
                        if (newOffset == -1) {
                            return node->keys[i][1];  // Возвращаем второй элемент найденного ключа

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
            // Поиск внутри листового узла
            for (int j = 0; j < (2 * t - 1); j++) {
                cnt += 2;
                if (key == node->keys[j][0]) {
                    if (newOffset == -1) {

                        return node->keys[j][1];  // Возвращаем второй элемент найденного ключа
                    }
                    else {
                        node->keys[j][1] = newOffset;
                        return node->keys[j][1];
                    }
                }
            }
            return -1;  // Возвращаем -1, если ключ не найден
        }
    }
    else {
        return -1;  // Возвращаем -1, если узел равен nullptr
    }
}

// Метод для удаления ключа из узла node
void Tree::removeFromNode(int key, BNode* node) {
    for (int i = 0; i < node->count; i++) {
        if (node->keys[i][0] == key) {
            // Сдвигаем ключи и потомков влево на одну позицию, начиная с найденной позиции
            for (int j = i; j < node->count; j++) {
                node->keys[j][0] = node->keys[j + 1][0];
                node->keys[j][1] = node->keys[j + 1][1];
                node->children[j] = node->children[j + 1];
            }
            // Обнуляем последний ключ и потомка
            node->keys[node->count - 1][0] = 0;
            node->keys[node->count - 1][1] = -1;
            node->children[node->count - 1] = node->children[node->count];
            node->children[node->count] = nullptr;
            // Уменьшаем количество ключей в узле
            node->count--;
            break;
        }
    }
}

// слияние двух узлов с правым и левым соответственно
// Метод для объединения двух узлов: node и othernode
void Tree::lconnect(BNode* node, BNode* othernode) {
    if (node == nullptr) return;
    // Переносим ключи и потомков из othernode в node
    for (int i = 0; i <= (othernode->count - 1); i++) {
        node->keys[node->count][0] = othernode->keys[i][0];
        node->keys[node->count][1] = othernode->keys[i][1];
        node->children[node->count] = othernode->children[i];
        node->count = node->count + 1;
    }
    // Переносим последнего потомка из othernode в node
    node->children[node->count] = othernode->children[othernode->count];
    // Обновляем parent для потомков в node
    for (int j = 0; j <= node->count; j++) {
        if (node->children[j] == nullptr) break;
        node->children[j]->parent = node;
    }
    // Удаляем othernode
    delete othernode;
}

// Метод для объединения двух узлов: node и othernode
void Tree::rconnect(BNode* node, BNode* othernode) {
    if (node == nullptr) return;

    // Переносим ключи и потомков из othernode в node
    for (int i = 0; i <= (othernode->count - 1); i++) {
        node->keys[node->count][0] = othernode->keys[i][0];
        node->keys[node->count][1] = othernode->keys[i][1];
        node->children[node->count + 1] = othernode->children[i + 1];
        node->count = node->count + 1;
    }
    // Обновляем parent для потомков в node
    for (int j = 0; j <= node->count; j++) {
        if (node->children[j] == nullptr) break;
        node->children[j]->parent = node;
    }
    // Удаляем othernode
    delete othernode;
}

// Метод ремонта дерева после удаления ключа
void Tree::repair(BNode* node) {
    // Если узел является корнем и пуст
    if ((node == root) && (node->count == 0)) {
        // Если у корня есть потомок, делаем его новым корнем
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

    // Находим позицию узла по отношению к родителю
    for (int j = 0; j <= parent->count; j++) {
        if (parent->children[j] == ptr) {
            positionSon = j;
            break;
        }
    }

    // Если ptr - первый ребенок (самый левый)
    if (positionSon == 0) {
        insert_to_node(parent->keys[positionSon][0], parent->keys[positionSon][1], ptr);
        lconnect(ptr, parent->children[positionSon + 1]);
        parent->children[positionSon + 1] = ptr;
        parent->children[positionSon] = nullptr;
        removeFromNode(parent->keys[positionSon][0], parent);

        // Если после вставки у ptr слишком много ключей, проводим реструктуризацию
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
        // Иначе, если у родителя недостаточно ключей, продолжаем ремонт
        else if (parent->count <= (t - 2)) {
            repair(parent);
        }
    }
    else {
        // Если ptr - последний ребенок (самый правый)
        if (positionSon == parent->count) {
            insert_to_node(parent->keys[positionSon - 1][0], parent->keys[positionSon - 1][1], parent->children[positionSon - 1]);
            lconnect(parent->children[positionSon - 1], ptr);
            parent->children[positionSon] = parent->children[positionSon - 1];
            parent->children[positionSon - 1] = nullptr;
            removeFromNode(parent->keys[positionSon - 1][0], parent);
            BNode* temp = parent->children[positionSon];

            // Если после вставки у temp слишком много ключей, проводим реструктуризацию
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
            // Иначе, если у родителя недостаточно ключей, продолжаем ремонт
            else if (parent->count <= (t - 2)) {
                repair(parent);
            }
        }
        else { // Если ptr имеет братьев справа и слева
            insert_to_node(parent->keys[positionSon][0], parent->keys[positionSon][1], ptr);
            lconnect(ptr, parent->children[positionSon + 1]);
            parent->children[positionSon + 1] = ptr;
            parent->children[positionSon] = nullptr;
            removeFromNode(parent->keys[positionSon][0], parent);

            // Если после вставки у ptr слишком много ключей, проводим реструктуризацию
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
            // Иначе, если у родителя недостаточно ключей, продолжаем ремонт
            else if (parent->count <= (t - 2)) {
                repair(parent);
            }
        }
    }
}

// метод удаления из листьев
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
            positionSon = j; //позиция узла по отношению к родителю
            break;
        }
    }
    //если ptr-первый ребенок (самый левый)
    if (positionSon == 0) {
        if (parent->children[positionSon + 1]->count > (t - 1)) { //если у правого брата больше, чем t-1 ключей
            k1 = parent->children[positionSon + 1]->keys[0][0]; //k1 - минимальный ключ правого брата
            k2 = parent->keys[positionSon][0]; //k2 - ключ родителя, больше, чем удаляемый, и меньше, чем k1
            int k2Offset = parent->keys[positionSon][1];
            insert_to_node(k2, k2Offset, ptr);
            removeFromNode(key, ptr);
            parent->keys[positionSon][0] = k1; //меняем местами k1 и k2
            removeFromNode(k1, parent->children[positionSon + 1]); //удаляем k1 из правого брата
        }
        else { //если у правого единственного брата не больше t-1 ключей		
            removeFromNode(key, ptr);
            if (ptr->count <= (t - 2)) repair(ptr);
        }
    }
    else {
        //если ptr-последний ребенок (самый правый)
        if (positionSon == parent->count) {
            //если у левого брата больше, чем t-1 ключей
            if (parent->children[positionSon - 1]->count > (t - 1)) {
                BNode* temp = parent->children[positionSon - 1];
                k1 = temp->keys[temp->count - 1][0]; //k1 - максимальный ключ левого брата
                k2 = parent->keys[positionSon - 1][0]; //k2 - ключ родителя, меньше, чем удаляемый и больше, чем k1
                int k2Offset = parent->keys[positionSon - 1][1];
                insert_to_node(k2, k2Offset, ptr);
                removeFromNode(key, ptr);
                parent->keys[positionSon - 1][0] = k1;
                removeFromNode(k1, temp);
            }
            else { //если у единственного левого брата не больше t-1 ключей
                removeFromNode(key, ptr);
                if (ptr->count <= (t - 2)) repair(ptr);
            }
        }
        else { //если ptr имеет братьев справа и слева
            //если у правого брата больше, чем t-1 ключей
            if (parent->children[positionSon + 1]->count > (t - 1)) {
                k1 = parent->children[positionSon + 1]->keys[0][0]; //k1 - минимальный ключ правого брата
                k2 = parent->keys[positionSon][0]; //k2 - ключ родителя, больше, чем удаляемый и меньше, чем k1
                int k2Offset = parent->keys[positionSon][1];
                insert_to_node(k2, k2Offset, ptr);
                removeFromNode(key, ptr);
                parent->keys[positionSon][0] = k1; //меняем местами k1 и k2
                removeFromNode(k1, parent->children[positionSon + 1]); //удаляем k1 из правого брата
            }
            else {
                //если у левого брата больше, чем t-1 ключей
                if (parent->children[positionSon - 1]->count > (t - 1)) {
                    BNode* temp = parent->children[positionSon - 1];
                    k1 = temp->keys[temp->count - 1][0]; //k1 - максимальный ключ левого брата
                    k2 = parent->keys[positionSon - 1][0]; //k2 - ключ родителя, меньше, чем удаляемый и больше, чем k1
                    int k2Offset = parent->keys[positionSon - 1][1];
                    insert_to_node(k2, k2Offset, ptr);
                    removeFromNode(key, ptr);
                    parent->keys[positionSon - 1][0] = k1;
                    removeFromNode(k1, temp);
                }
                else { //если у обоих братьев не больше t-1 ключей
                    removeFromNode(key, ptr);
                    if (ptr->count <= (t - 2)) repair(ptr);
                }
            }
        }
    }
}

// метод удаления для всех узлов кроме листовых
void Tree::remove(int key, BNode* node) {
    BNode* ptr = node;
    int position; //номер ключа
    int i;
    for (int i = 0; i <= node->count - 1; i++) {
        if (key == node->keys[i][0]) {
            position = i;
            break;
        }
    }
    int positionSon; //номер сына по отношению к родителю
    if (ptr->parent != nullptr) {
        for (int i = 0; i <= ptr->parent->count; i++) {
            if (ptr->children[i] == ptr) {
                positionSon == i;
                break;
            }
        }
    }
    //находим наименьший ключ правого поддерева
    ptr = ptr->children[position + 1];
    int newkey = ptr->keys[0][0];
    while (ptr->leaf == false) ptr = ptr->children[0];
    //если ключей в найденном листе не больше 1 - ищем наибольший ключ в левом поддереве
    //иначе - просто заменяем key на новый ключ, удаляем новый ключ из листа
    if (ptr->count > (t - 1)) {
        newkey = ptr->keys[0][0];
        removeFromNode(newkey, ptr);
        node->keys[position][0] = newkey;
    }
    else {
        ptr = node;
        //ищем наибольший ключ в левом поддереве
        ptr = ptr->children[position];
        newkey = ptr->keys[ptr->count - 1][0];
        while (ptr->leaf == false) ptr = ptr->children[ptr->count];
        newkey = ptr->keys[ptr->count - 1][0];
        node->keys[position][0] = newkey;
        if (ptr->count > (t - 1)) removeFromNode(newkey, ptr);
        else {
            //если ключей не больше, чем t-1 - перестраиваем
            removeLeaf(newkey, ptr);
        }
    }
}

// Основной метод удаления, из которого вызываются все методы удаления
void Tree::remove(int key) {
    BNode* ptr = this->root;  // Указатель на корень дерева
    int position;  // Позиция ключа в узле
    int positionSon;  // Позиция ребенка узла
    int i;

    // Если ключ не найден, завершаем операцию удаления
    if (searchKey(key, ptr) == -1) {
        return;
    }
    else {
        // Ищем узел, в котором находится ключ для удаления
        for (i = 0; i <= ptr->count - 1; i++) {
            if (ptr->keys[i][0] != 0) {
                // Если ключ найден, сохраняем его позицию и выходим из цикла
                if (key == ptr->keys[i][0]) {
                    position = i;
                    break;
                }
                else {
                    // Если ключ меньше текущего, переходим к левому потомку
                    if ((key < ptr->keys[i][0])) {
                        ptr = ptr->children[i];
                        positionSon = i;
                        i = -1;
                    }
                    else {
                        // Если достигли конца узла, переходим к правому потомку
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
    // Если узел является листом
    if (ptr->leaf == true) {
        // Если в узле достаточно ключей, вызываем метод removeFromNode
        if (ptr->count > (t - 1)) {
            removeFromNode(key, ptr);
        }
        // Иначе вызываем метод removeLeaf
        else {
            removeLeaf(key, ptr);
        }
    }
    // Если узел не является листом, вызываем рекурсивный метод remove
    else {
        remove(key, ptr);
    }
}

// Метод вставляет все записи из файла в B-дерево
void insertAll(FileManager fm, Tree* tree) {
    ifstream fin(fm.filename, ios_base::binary);  // Открываем файл для чтения в бинарном режиме
    // Проходим по всем записям в файле
    for (int i = 0; i < fm.cnt; i++) {
        int offSet = fm.lastOffset;  // Получаем текущее смещение
        // Получаем запись из файла по текущему смещению
        FlightRecord record = fm.getFlightRecord(offSet, fin);
        // Вставляем запись в дерево
        tree->insert(record.flight_number, offSet);
    }
    fin.close();  // Закрываем файл после использования
}

void displayFindNum(FileManager fm, Tree* tree) {
    // Пользователь вводит номер для поиска
    cout << "Введите номер для поиска: ";
    int num;
    cin >> num;

    // Открываем файл для чтения в бинарном режиме
    ifstream fin("test.bin", ios::binary);
    /*auto start_time = chrono::high_resolution_clock::now();*/
    // Ищем номер в дереве
    int offset = tree->search(num);
    /* auto end_time = chrono::high_resolution_clock::now();
     auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
     cout << "Время выполнения функции findInFile: " << duration.count() << " мк" << endl;*/
     // Если номер не найден, выводим сообщение и завершаем функцию
    if (offset == -1) {
        cout << "Номер не найден." << endl;
        return;
    }
    // Получаем запись о полете по смещению и выводим ее на экран
    FlightRecord record = fm.getFlightRecord(offset, fin);
    cout << record.departure_point << " " << record.flight_number << " " << record.arrival_date <<
        " " << record.arrival_time << " " << record.delay_hours << endl;
    // Закрываем файл
    fin.close();

}

void deleteNode(FileManager& fm, Tree* tree) {
    ifstream fin("test.bin", ios::binary);
    cout << "Введите номер для удаления: ";
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
        cerr << "Ошибка открытия файлов." << endl;
    }
    for (int i = 0; i < fm.cnt; i++)
    {
        if (i == skip_offset) {
            continue;
        }
        FlightRecord tmp; // Создаем объект структуры Record для хранения данных.

        fm.lastOffset = tempFile.tellp();

        fin.seekg(i * 37); // Устанавливаем позицию чтения в файле на указанное смещение.

        string str = ""; // Создаем строку для хранения данных о название кинотеатра.
        char ch;
        fin.get(ch);
        while (ch != '\0') {
            str += ch; // Считываем символы до символа '\0' (нулевого символа) и добавляем их к строке namecinemaa.
            fin.get(ch);
        }
        tmp.departure_point = str; // Присваиваем полученную строку полю cname объекта tmp.
        // Чтение данных из файла
        fin.read((char*)&tmp.flight_number, sizeof(int)); // Считываем целое число (int) и сохраняем его в поле uncode.

        str = ""; // Создаем строку для хранения данных о названии фильма.
        fin.get(ch);
        while (ch != '\0') {
            str += ch; // Считываем символы до символа '\0' (нулевого символа) и добавляем их к строке filmnamee.
            fin.get(ch);
        }
        tmp.arrival_date = str; // Присваиваем полученную строку полю fname объекта tmp.

        str = ""; // Создаем строку для хранения данных о названии фильма.
        fin.get(ch);
        while (ch != '\0') {
            str += ch; // Считываем символы до символа '\0' (нулевого символа) и добавляем их к строке filmnamee.
            fin.get(ch);
        }
        tmp.arrival_time = str; // Присваиваем полученную строку полю fname объекта tmp.

        fin.read((char*)&tmp.delay_hours, sizeof(double));

        //название места вылета
        const char* cinemaname = tmp.departure_point.c_str();
        int cinemasize = tmp.departure_point.size();
        tempFile.write(cinemaname, cinemasize + 1);

        tempFile.write((char*)&tmp.flight_number, sizeof(int));

        //название фильма
        const char* filmname = tmp.arrival_date.c_str();
        int filmsize = tmp.arrival_date.size();
        tempFile.write(filmname, filmsize + 1);

        //Дата начала сеанса
        const char* date = tmp.arrival_time.c_str();
        int datelength = tmp.arrival_time.size();
        tempFile.write(date, datelength + 1);

        tempFile.write((char*)&tmp.delay_hours, sizeof(double));

        tree->search(tmp.flight_number, fm.lastOffset);
    }

    // Закрыть файлы
    fin.close();
    tempFile.close();
    if (remove(filename.c_str()) != 0) {
        cerr << "Ошибка при удалении исходного файла." << endl;
    }
    // Заменить исходный файл временным
    if (rename(tempFilename.c_str(), filename.c_str()) != 0) {
        cerr << "Ошибка при переименовании файлов." << endl;
    }
    cout << "Строка удалена успешно." << endl;
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