#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace chrono;

// ЧАСТЬ 1: ДВОИЧНЫЙ ПОИСК ДЛЯ STRING

// Лексикографический поиск (по алфавиту)
int binarySearchString(const vector<string>& arr, const string& target) {
    int left = 0, right = arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;        // Нашли
        if (arr[mid] < target) left = mid + 1;    // Ищем справа
        else right = mid - 1;                      // Ищем слева
    }
    return -1; // Не нашли
}

// ЧАСТЬ 2: ХЕШ-ТАБЛИЦА С МЕТОДОМ ЦЕПОЧЕК
// Ключ: int, Значение: string
template <typename K, typename V>
class HashTableChaining {
private:
    struct Node {
        K key;
        V value;
        Node* next;
        Node(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };

    vector<Node*> table;
    int size;
    int count;

    int hash(const K& key) const {
        return key % size;
    }

public:
    HashTableChaining(int s = 10) : size(s), count(0) {
        table.resize(size, nullptr);
    }

    ~HashTableChaining() {
        for (int i = 0; i < size; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    void insert(const K& key, const V& value) {
        int index = hash(key);
        // Проверяем, есть ли уже такой ключ
        Node* current = table[index];
        while (current != nullptr) {
            if (current->key == key) {
                current->value = value; // Обновляем
                return;
            }
            current = current->next;
        }
        // Вставляем в начало цепочки
        Node* newNode = new Node(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        count++;
    }

    bool find(const K& key, V& result) const {
        int index = hash(key);
        Node* current = table[index];
        while (current != nullptr) {
            if (current->key == key) {
                result = current->value;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void print() const {
        for (int i = 0; i < size; i++) {
            cout << i << ": ";
            Node* current = table[i];
            while (current != nullptr) {
                cout << "(" << current->key << ", " << current->value << ") ";
                current = current->next;
            }
            cout << endl;
        }
    }
};

// ЧАСТЬ 3: СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ
int main() {
    setlocale(LC_ALL, "ru");

    cout << "ЛР №8: Двоичный поиск и хеш-таблицы\n";

    cout << "\n1. Двоичный поиск (string)\n";
    vector<string> words = { "apple", "banana", "cherry", "date", "elderberry", "fig", "grape" };
    string target = "date";
    int idx = binarySearchString(words, target);
    if (idx != -1) cout << "Слово \"" << target << "\" найдено на позиции " << idx << endl;
    else cout << "Слово \"" << target << "\" не найдено" << endl;

    cout << "\n2. Хеш-таблица (цепочки)\n";
    HashTableChaining<int, string> ht(5);
    ht.insert(12, "двенадцать");   // 12 % 5 = 2
    ht.insert(23, "двадцать три"); // 23 % 5 = 3
    ht.insert(31, "тридцать один");// 31 % 5 = 1
    ht.insert(15, "пятнадцать");   // 15 % 5 = 0
    ht.insert(27, "двадцать семь");// 27 % 5 = 2 (коллизия с 12)
    ht.print();

    string value;
    if (ht.find(23, value)) cout << "Ключ 23: " << value << endl;
    if (ht.find(99, value)) cout << "Ключ 99: " << value << endl;
    else cout << "Ключ 99 не найден" << endl;

    cout << "\n3. Сравнение двоичного поиска и хеш-таблицы\n";
    cout << "Размер\tДвоичный поиск (мкс)\tХеш-таблица (мкс)\n";

    vector<int> sizes = { 1000, 10000, 100000 };
    for (int N : sizes) {
        // Создаём отсортированный массив и хеш-таблицу
        vector<int> arr(N);
        HashTableChaining<int, int> table(N * 2);
        for (int i = 0; i < N; i++) {
            arr[i] = i;
            table.insert(i, i);
        }

        // Генерируем 1000 запросов
        vector<int> queries;
        for (int i = 0; i < 1000; i++) queries.push_back(rand() % N);

        // Замер двоичного поиска
        auto start = high_resolution_clock::now();
        for (int q : queries) {
            int left = 0, right = N - 1;
            while (left <= right) {
                int mid = left + (right - left) / 2;
                if (arr[mid] == q) break;
                if (arr[mid] < q) left = mid + 1;
                else right = mid - 1;
            }
        }
        auto end = high_resolution_clock::now();
        long long binaryTime = duration_cast<microseconds>(end - start).count();

        // Замер хеш-таблицы
        start = high_resolution_clock::now();
        for (int q : queries) {
            int val;
            table.find(q, val);
        }
        end = high_resolution_clock::now();
        long long hashTime = duration_cast<microseconds>(end - start).count();

        cout << N << "\t" << binaryTime << "\t\t\t" << hashTime << endl;
    }

    return 0;
}