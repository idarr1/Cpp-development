#include <iostream>
#include <map>
#include <vector>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
using namespace chrono;

// КЛАСС SparseArray<T> — Разреженный массив (хранит только не нули)
template <typename T>
class SparseArray {
private:
    map<int, T> data;
    int totalSize;

public:
    SparseArray(int size) : totalSize(size) {
        //cout << "[+] Создан SparseArray размером " << totalSize << endl;
    }

    ~SparseArray() {}

    // Сложность: O(log n)
    void set(int index, T value) {
        if (index >= 0 && index < totalSize) {
            if (value == T()) data.erase(index);
            else data[index] = value;
        }
    }

    // Сложность: O(log n)
    T get(int index) {
        if (index >= 0 && index < totalSize) {
            if (data.find(index) != data.end()) return data[index];
        }
        return T(); // Возвращаем 0, если нет
    }

    // Сложность: O(log n)
    void remove(int index) {
        if (index >= 0 && index < totalSize) {
            data.erase(index);
        }
    }

    // Сложность: O(1)
    int size() { return totalSize; }
    int nonZeroCount() { return data.size(); }

    // Сложность: O(k), где k - кол-во ненулевых
    void print() {
        cout << "SparseArray (всего: " << totalSize << ", ненулевых: " << data.size() << "):" << endl;
        for (auto const& [key, val] : data) {
            cout << "  [" << key << "] = " << val << endl;
        }
        if (data.empty()) cout << "  (пусто)" << endl;
    }
};

int main() {
    // Настройка кодировки UTF-8
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif
    setlocale(LC_ALL, "ru");

    cout << "ЛР №2: SparseArray<T>" << endl;

    // 1. Демонстрация
    cout << "\n1. Демонстрация" << endl;
    SparseArray<int> arr(10);
    arr.set(2, 5);
    arr.set(5, 10);
    arr.set(7, 3);
    arr.print();

    cout << "\nget(2) = " << arr.get(2) << endl;
    cout << "get(1) = " << arr.get(1) << " (ноль)" << endl;

    arr.remove(5);
    cout << "\nПосле удаления [5]:" << endl;
    arr.print();

    // 2. Анализ сложности
    cout << "\n2. Анализ сложности" << endl;
    cout << "set: O(log n)" << endl;
    cout << "get: O(log n)" << endl;
    cout << "remove: O(log n)" << endl;
    cout << "size: O(1)" << endl;
    cout << "print: O(k)" << endl;

    // 3. Сравнение с std::map
    cout << "\n3. Сравнение с STL std::map" << endl;
    cout << "Размер\tSparseArray (мкс)\tstd::map (мкс)" << endl;

    vector<int> sizes = { 1000, 10000, 50000 };
    for (int n : sizes) {
        // Замер SparseArray
        SparseArray<int> sparse(n);
        auto start1 = high_resolution_clock::now();
        for (int i = 0; i < n; i += 100) sparse.set(i, i);
        auto end1 = high_resolution_clock::now();
        long long timeSparse = duration_cast<microseconds>(end1 - start1).count();

        // Замер std::map
        map<int, int> stlMap;
        auto start2 = high_resolution_clock::now();
        for (int i = 0; i < n; i += 100) stlMap[i] = i;
        auto end2 = high_resolution_clock::now();
        long long timeMap = duration_cast<microseconds>(end2 - start2).count();

        cout << n << "\t" << timeSparse << "\t\t\t" << timeMap << endl;
    }

    return 0;
}