#include <iostream>
#include <stdexcept>
#include <functional>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// ШАБЛОННЫЙ КЛАСС Vector<T>
template <typename T>
class Vector {
private:
    T* data;    // Указатель на динамический массив
    int sz;     // Текущий размер
    int cap;    // Ёмкость

public:
    Vector() : data(nullptr), sz(0), cap(0) {}

    explicit Vector(int n) : sz(n), cap(n) {
        data = new T[cap];
        for (int i = 0; i < sz; i++) data[i] = T();
    }

    // Конструктор копирования (правило трёх)
    Vector(const Vector& other) : sz(other.sz), cap(other.cap) {
        data = new T[cap];
        for (int i = 0; i < sz; i++) data[i] = other.data[i];
    }

    // Конструктор перемещения (правило пяти)
    Vector(Vector&& other) noexcept : data(other.data), sz(other.sz), cap(other.cap) {
        other.data = nullptr;
        other.sz = 0;
        other.cap = 0;
    }

    // Деструктор
    ~Vector() { delete[] data; }

    // Операторы
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector temp(other); // copy-and-swap
            swap(temp);
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            sz = other.sz;
            cap = other.cap;
            other.data = nullptr;
            other.sz = 0;
            other.cap = 0;
        }
        return *this;
    }

    T& operator[](int index) {
        if (index < 0 || index >= sz) throw out_of_range("Index out of range");
        return data[index];
    }
    const T& operator[](int index) const {
        if (index < 0 || index >= sz) throw out_of_range("Index out of range");
        return data[index];
    }

    // Методы доступа
    int size() const { return sz; }
    int capacity() const { return cap; }
    bool empty() const { return sz == 0; }

    // Резервирование памяти
    void reserve(int newCap) {
        if (newCap <= cap) return;
        T* newData = new T[newCap];
        for (int i = 0; i < sz; i++) newData[i] = data[i];
        delete[] data;
        data = newData;
        cap = newCap;
    }

    // Добавление в конец (амортизированно O(1))
    void push_back(const T& value) {
        if (sz >= cap) {
            int newCap = cap == 0 ? 1 : cap * 2;
            reserve(newCap);
        }
        data[sz++] = value;
    }

    // Удаление последнего
    void pop_back() {
        if (empty()) throw underflow_error("Vector is empty");
        sz--;
    }

    // Изменение размера
    void resize(int newSize, const T& defaultValue = T()) {
        if (newSize < sz) {
            sz = newSize;
        }
        else if (newSize > sz) {
            if (newSize > cap) reserve(newSize);
            for (int i = sz; i < newSize; i++) data[i] = defaultValue;
            sz = newSize;
        }
    }

    void clear() { sz = 0; }

    void swap(Vector& other) noexcept {
        std::swap(data, other.data);
        std::swap(sz, other.sz);
        std::swap(cap, other.cap);
    }

    // Итераторы
    T* begin() { return data; }
    T* end() { return data + sz; }
    const T* begin() const { return data; }
    const T* end() const { return data + sz; }

    // ДОП. ФУНКЦИОНАЛЬНОСТЬ (Вариант 7): filter
    Vector<T> filter(function<bool(const T&)> predicate) const {
        Vector<T> result;
        for (int i = 0; i < sz; i++) {
            if (predicate(data[i])) {
                result.push_back(data[i]);
            }
        }
        return result;
    }
};

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    setlocale(LC_ALL, "ru");

    cout << "ЛР №3: Шаблонный класс Vector<T>" << endl;

    // 1. Создание и заполнение
    cout << "1. Создание и push_back" << endl;
    Vector<int> v;
    for (int i = 1; i <= 10; i++) {
        v.push_back(i * 10);
        cout << "push_back(" << i * 10 << "): размер=" << v.size()
            << ", ёмкость=" << v.capacity() << endl;
    }

    cout << "\nСодержимое: ";
    for (int x : v) cout << x << " ";
    cout << endl;

    // 2. Доступ по индексу
    cout << "\n2. Доступ по индексу" << endl;
    cout << "v[0] = " << v[0] << ", v[9] = " << v[9] << endl;

    // 3. Фильтрация
    cout << "\n3. filter: элементы > 50" << endl;
    Vector<int> filtered = v.filter([](const int& x) { return x > 50; });
    cout << "Отфильтровано: ";
    for (int x : filtered) cout << x << " ";
    cout << "\nРазмер: " << filtered.size() << endl;

    // 4. Копирование (правило трёх)
    cout << "\n4. Копирование" << endl;
    Vector<int> v2 = v;
    cout << "v2 (копия): ";
    for (int x : v2) cout << x << " ";
    cout << endl;

    // 5. Перемещение (правило пяти)
    cout << "\n5. Перемещение" << endl;
    Vector<int> v3 = std::move(v2);
    cout << "v3 (перемещённый): ";
    for (int x : v3) cout << x << " ";
    cout << "\nv2 после перемещения, размер: " << v2.size() << endl;

    // 6. resize и pop_back
    cout << "\n6. resize и pop_back" << endl;
    v.resize(15, 0);
    cout << "После resize(15, 0): размер=" << v.size() << endl;
    v.pop_back();
    cout << "После pop_back: размер=" << v.size() << endl;

    return 0;
}