#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <locale>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// КЛАСС PRODUCT (ТОВАР)
// Отношение к Warehouse: Агрегация (товар живет отдельно от склада)
class Product {
private:
    string name;       
    double price;      
    int quantity;      

    static int productCount;

public:
    // Конструктор: вызывается при создании объекта
    Product(string n, double p, int q) : name(n), price(p), quantity(q) {
        productCount++;
        cout << "[+] Создан товар: " << name << endl;
    }

    // Деструктор: вызывается при удалении объекта
    ~Product() {
        productCount--;
        cout << "[-] Удален товар: " << name << endl;
    }

    void print() const {
        cout << "  - " << name << " | Цена: " << price
            << " | Кол-во: " << quantity << " шт." << endl;
    }

    static int getCount() {
        return productCount;
    }
};

int Product::productCount = 0;

// КЛАСС SUPPLIER (ПОСТАВЩИК)
// Отношение к Warehouse: Агрегация (поставщик живет отдельно от склада)
class Supplier {
private:
    string name;       
    string contact;    

public:
    // Конструктор
    Supplier(string n, string c) : name(n), contact(c) {
        cout << "[+] Создан поставщик: " << name << endl;
    }

    // Деструктор
    ~Supplier() {
        cout << "[-] Удален поставщик: " << name << endl;
    }

    void print() const {
        cout << "  - " << name << " | Контакт: " << contact << endl;
    }
};

// КЛАСС WAREHOUSE (СКЛАД)
// Отношение к Product/Supplier: Агрегация (хранит указатели)
// Паттерн: Singleton (только один склад)
class Warehouse {
private:
    static Warehouse* instance;

    string address;              // Адрес склада
    vector<Product*> products;   // Храним указатели (агрегация)
    vector<Supplier*> suppliers; // Храним указатели (агрегация)

    // Приватный конструктор: нельзя создать склад напрямую (new Warehouse)
    Warehouse(string addr) : address(addr) {
        cout << "[!] Открыт склад по адресу: " << address << endl;
    }

public:
    // Запрещаем копирование (обязательно для Singleton)
    Warehouse(const Warehouse&) = delete;
    Warehouse& operator=(const Warehouse&) = delete;

    static Warehouse* getInstance(string addr = "ул. Центральная, д. 1") {
        if (instance == nullptr) {
            instance = new Warehouse(addr);
        }
        return instance;
    }

    // Деструктор склада
    ~Warehouse() {
        cout << "[!] Закрыт склад по адресу: " << address << endl;
        cout << "    ВНИМАНИЕ: Товары и поставщики остаются живы (агрегация)!" << endl;
    }

    // Добавление товара
    void addProduct(Product* p) {
        if (p == nullptr) {
            cout << "[ОШИБКА] Нельзя добавить nullptr вместо товара!" << endl;
            return;
        }
        products.push_back(p);
        cout << "    Товар добавлен на склад." << endl;
    }

    // Добавление поставщика
    void addSupplier(Supplier* s) {
        if (s == nullptr) {
            cout << "[ОШИБКА] Нельзя добавить nullptr вместо поставщика!" << endl;
            return;
        }
        suppliers.push_back(s);
        cout << "    Поставщик добавлен на склад." << endl;
    }

    // Вывод всех товаров на складе
    void printProducts() const {
        cout << "\n--- Товары на складе (" << products.size() << " шт.) ---" << endl;
        if (products.empty()) {
            cout << "  (пусто)" << endl;
            return;
        }
        for (const auto* p : products) {
            p->print();
        }
    }

    // Вывод всех поставщиков
    void printSuppliers() const {
        cout << "\n--- Поставщики (" << suppliers.size() << " шт.) ---" << endl;
        if (suppliers.empty()) {
            cout << "  (пусто)" << endl;
            return;
        }
        for (const auto* s : suppliers) {
            s->print();
        }
    }
};

Warehouse* Warehouse::instance = nullptr;

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8); // Windows-1251
        SetConsoleCP(CP_UTF8);
    #endif
    setlocale(LC_ALL, "ru");

    cout << "ЛР №1: Система учёта поставок\n" << endl;

    // 1. Создаем товары 
    cout << "1. Создание товаров:" << endl;
    Product p1("Ноутбук", 75000, 5);
    Product p2("Мышь", 1500, 20);
    Product p3("Клавиатура", 3500, 10);

    // 2. Создаем поставщиков 
    cout << "\n2. Создание поставщиков:" << endl;
    Supplier s1("ООО Ромашка", "info@romashka.ru");
    Supplier s2("ИП Иванов", "+7-999-123-45-67");

    // 3. Получаем склад
    cout << "\n3. Получение склада (Singleton):" << endl;
    Warehouse* w1 = Warehouse::getInstance("ул. Складская, д. 5");
    Warehouse* w2 = Warehouse::getInstance(); // Вызываем второй раз

    // Проверка Singleton
    if (w1 == w2) {
        cout << "    [ТЕСТ] Singleton работает: w1 и w2 — это один и тот же объект!" << endl;
    }

    // 4. Добавляем товары и поставщиков на склад
    cout << "\n4. Добавление на склад:" << endl;
    w1->addProduct(&p1);
    w1->addProduct(&p2);
    w1->addProduct(&p3);
    w1->addProduct(nullptr); // Тест на защиту от nullptr

    w1->addSupplier(&s1);
    w1->addSupplier(&s2);

    // 5. Выводим содержимое склада
    w1->printProducts();
    w1->printSuppliers();

    // 6. Вывод статического счетчика
    cout << "\n5. Статический счетчик:" << endl;
    cout << "    Всего создано товаров: " << Product::getCount() << endl;

    // 7. Удаляем склад (проверка агрегации)
    cout << "\n6. Удаление склада:" << endl;
    delete w1;

    // 8. Проверяем, что товары ЖИВЫ после удаления склада
    cout << "\n7. Проверка агрегации (товары живы):" << endl;
    cout << "    Товары все еще в памяти!" << endl;
    p1.print();
    p2.print();
    p3.print();

    cout << "\nКонец программы" << endl;
    return 0;
}