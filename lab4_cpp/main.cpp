#include <iostream>
#include <stdexcept>
#include <windows.h>

using namespace std;

// ОДНОСВЯЗНЫЙ СПИСОК
template <typename T>
class SinglyLinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    // Конструктор
    SinglyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    // Деструктор
    ~SinglyLinkedList() { clear(); }

    // Вставка в начало (O(1))
    void push_front(const T& value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
        if (tail == nullptr) tail = newNode;
        count++;
    }

    // Вставка в конец (O(1) с tail)
    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
        count++;
    }

    // Удаление из начала (O(1))
    void pop_front() {
        if (head == nullptr) throw underflow_error("Список пуст");
        Node* temp = head;
        head = head->next;
        delete temp;
        count--;
        if (head == nullptr) tail = nullptr;
    }

    // Удаление из конца (O(n))
    void pop_back() {
        if (head == nullptr) throw underflow_error("Список пуст");
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        }
        else {
            Node* current = head;
            while (current->next != tail) {
                current = current->next;
            }
            delete tail;
            tail = current;
            tail->next = nullptr;
        }
        count--;
    }

    // Поиск (O(n))
    Node* find(const T& value) const {
        Node* current = head;
        while (current != nullptr) {
            if (current->data == value) return current;
            current = current->next;
        }
        return nullptr;
    }

    // Удаление по значению (O(n))
    bool remove(const T& value) {
        if (head == nullptr) return false;
        if (head->data == value) {
            pop_front();
            return true;
        }
        Node* current = head;
        while (current->next != nullptr && current->next->data != value) {
            current = current->next;
        }
        if (current->next == nullptr) return false;
        Node* toDelete = current->next;
        current->next = toDelete->next;
        if (toDelete == tail) tail = current;
        delete toDelete;
        count--;
        return true;
    }

    // Вывод (O(n))
    void print() const {
        Node* current = head;
        cout << "[";
        while (current != nullptr) {
            cout << current->data;
            if (current->next != nullptr) cout << ", ";
            current = current->next;
        }
        cout << "]" << endl;
    }

    // Очистка
    void clear() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        count = 0;
    }

    int size() const { return count; }
    bool empty() const { return head == nullptr; }
};

// ДВУСВЯЗНЫЙ СПИСОК
template <typename T>
class DoublyLinkedList {
private:
    struct DNode {
        T data;
        DNode* prev;
        DNode* next;
        DNode(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };

    DNode* head;
    DNode* tail;
    int count;

public:
    // Конструктор
    DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    // Деструктор
    ~DoublyLinkedList() { clear(); }

    // Вставка в начало (O(1))
    void push_front(const T& value) {
        DNode* newNode = new DNode(value);
        if (head == nullptr) {
            head = tail = newNode;
        }
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        count++;
    }

    // Вставка в конец (O(1))
    void push_back(const T& value) {
        DNode* newNode = new DNode(value);
        if (tail == nullptr) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    // Удаление из начала (O(1))
    void pop_front() {
        if (head == nullptr) throw underflow_error("Список пуст");
        DNode* temp = head;
        head = head->next;
        if (head != nullptr) head->prev = nullptr;
        else tail = nullptr;
        delete temp;
        count--;
    }

    // Удаление из конца (O(1))
    void pop_back() {
        if (tail == nullptr) throw underflow_error("Список пуст");
        DNode* temp = tail;
        tail = tail->prev;
        if (tail != nullptr) tail->next = nullptr;
        else head = nullptr;
        delete temp;
        count--;
    }

    // Поиск (O(n))
    DNode* find(const T& value) const {
        DNode* current = head;
        while (current != nullptr) {
            if (current->data == value) return current;
            current = current->next;
        }
        return nullptr;
    }

    // Обход в обратном порядке (O(n))
    void printReverse() const {
        DNode* current = tail;
        cout << "[";
        while (current != nullptr) {
            cout << current->data;
            if (current->prev != nullptr) cout << ", ";
            current = current->prev;
        }
        cout << "]" << endl;
    }

    // Вывод (O(n))
    void print() const {
        DNode* current = head;
        cout << "[";
        while (current != nullptr) {
            cout << current->data;
            if (current->next != nullptr) cout << ", ";
            current = current->next;
        }
        cout << "]" << endl;
    }

    // Очистка
    void clear() {
        while (head != nullptr) {
            DNode* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        count = 0;
    }

    int size() const { return count; }
    bool empty() const { return head == nullptr; }
};

int main() {
    setlocale(LC_ALL, "ru");

    cout << "ЛР №4: Связные списки" << endl;

    cout << "\n1. Односвязный список" << endl;
    SinglyLinkedList<int> sll;
    sll.push_back(10);
    sll.push_back(20);
    sll.push_back(30);
    sll.push_front(5);
    cout << "Список: ";
    sll.print();
    cout << "Размер: " << sll.size() << endl;

    sll.pop_front();
    cout << "После pop_front: ";
    sll.print();

    sll.pop_back();
    cout << "После pop_back: ";
    sll.print();

    cout << "\n2. Двусвязный список" << endl;
    DoublyLinkedList<int> dll;
    for (int i = 1; i <= 5; i++) dll.push_back(i * 10);
    cout << "Прямой порядок: ";
    dll.print();
    cout << "Обратный порядок: ";
    dll.printReverse();

    dll.pop_back();
    cout << "После pop_back: ";
    dll.print();

    cout << "\n3. Проверка исключений" << endl;
    try {
        SinglyLinkedList<int> emptyList;
        emptyList.pop_front();
    }
    catch (const underflow_error& e) {
        cout << "Исключение: " << e.what() << endl;
    }

    return 0;
}