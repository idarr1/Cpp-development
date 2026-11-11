#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <windows.h>

using namespace std;

// 1. СТЕК (Stack) — LIFO (Last In, First Out)
template <typename T>
class Stack {
private:
    T* data;       
    int topIndex;  
    int capacity; 

    void resize() {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i <= topIndex; i++) newData[i] = data[i];
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    Stack(int initialCapacity = 10) : capacity(initialCapacity), topIndex(-1) {
        data = new T[capacity];
    }

    ~Stack() { delete[] data; }

    // Добавление элемента в стек
    void push(const T& value) {
        if (topIndex >= capacity - 1) resize();
        data[++topIndex] = value;
    }

    // Удаление и возврат верхнего элемента
    T pop() {
        if (isEmpty()) throw underflow_error("Стек пуст");
        return data[topIndex--]; 
    }

    // Просмотр верхнего элемента без удаления
    T top() const {
        if (isEmpty()) throw underflow_error("Стек пуст");
        return data[topIndex];
    }

    bool isEmpty() const { return topIndex == -1; }
    int size() const { return topIndex + 1; } 

    // ЗАДАЧА: Удалить все чётные элементы из стека
    void removeEven() {
        Stack<T> temp; // Временный стек
        while (!isEmpty()) {
            T val = pop();
            // Если элемент нечётный — оставляем во временном стеке
            if (val % 2 != 0) temp.push(val);
        }
        // Возвращаем элементы обратно в исходный стек
        while (!temp.isEmpty()) push(temp.pop());
    }

    void print() const {
        cout << "[";
        for (int i = 0; i <= topIndex; i++) {
            cout << data[i];
            if (i < topIndex) cout << ", ";
        }
        cout << "]" << endl;
    }
};

// 2. ОЧЕРЕДЬ (Queue) — FIFO (First In, First Out)
template <typename T>
class Queue {
private:
    T* data;        
    int frontIndex; 
    int rearIndex;  
    int count;      
    int capacity;   

    void resize() {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i < count; i++) {
            newData[i] = data[(frontIndex + i) % capacity];
        }
        delete[] data;
        data = newData;
        frontIndex = 0;
        rearIndex = count;
        capacity = newCapacity;
    }

public:
    Queue(int initialCapacity = 10) : capacity(initialCapacity), frontIndex(0), rearIndex(0), count(0) {
        data = new T[capacity];
    }

    ~Queue() { delete[] data; }

    // Добавление элемента в конец очереди
    void enqueue(const T& value) {
        if (count >= capacity) resize();
        data[rearIndex] = value;
        rearIndex = (rearIndex + 1) % capacity; 
        count++;
    }

    // Удаление и возврат элемента из начала
    T dequeue() {
        if (isEmpty()) throw underflow_error("Очередь пуста");
        T value = data[frontIndex];
        frontIndex = (frontIndex + 1) % capacity; 
        count--;
        return value;
    }

    // Просмотр первого элемента
    T front() const {
        if (isEmpty()) throw underflow_error("Очередь пуста");
        return data[frontIndex];
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }

    // ЗАДАЧА: Сортировка очереди
    void sortQueue() {
        // Переливаем элементы в вектор
        std::vector<T> vec;
        while (!isEmpty()) {
            vec.push_back(dequeue());
        }
        // Сортируем вектор
        for (size_t i = 0; i < vec.size(); i++) {
            for (size_t j = i + 1; j < vec.size(); j++) {
                if (vec[i] > vec[j]) {
                    std::swap(vec[i], vec[j]);
                }
            }
        }
        // Возвращаем обратно в очередь
        for (const T& val : vec) {
            enqueue(val);
        }
    }

    void print() const {
        cout << "[";
        for (int i = 0; i < count; i++) {
            cout << data[(frontIndex + i) % capacity];
            if (i < count - 1) cout << ", ";
        }
        cout << "]" << endl;
    }
};

// 3. ДЕК (Deque) — добавление/удаление с обоих концов
template <typename T>
class Deque {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node* head; 
    Node* tail; 
    int count;  

public:
    Deque() : head(nullptr), tail(nullptr), count(0) {}

    ~Deque() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Добавление в начало
    void push_front(const T& value) {
        Node* newNode = new Node(value);
        if (head == nullptr) head = tail = newNode;
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        count++;
    }

    // Добавление в конец
    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (tail == nullptr) head = tail = newNode;
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    // Удаление из начала
    T pop_front() {
        if (isEmpty()) throw underflow_error("Дек пуст");
        Node* temp = head;
        T value = temp->data;
        head = head->next;
        if (head != nullptr) head->prev = nullptr;
        else tail = nullptr;
        delete temp;
        count--;
        return value;
    }

    // Удаление из конца
    T pop_back() {
        if (isEmpty()) throw underflow_error("Дек пуст");
        Node* temp = tail;
        T value = temp->data;
        tail = tail->prev;
        if (tail != nullptr) tail->next = nullptr;
        else head = nullptr;
        delete temp;
        count--;
        return value;
    }

    T front() const {
        if (isEmpty()) throw underflow_error("Дек пуст");
        return head->data;
    }

    T back() const {
        if (isEmpty()) throw underflow_error("Дек пуст");
        return tail->data;
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }

    // ЗАДАЧА: Удалить все чётные элементы из дека
    void removeEven() {
        Deque<T> temp;
        while (!isEmpty()) {
            T val = pop_front();
            if (val % 2 != 0) temp.push_back(val); // Оставляем нечётные
        }
        while (!temp.isEmpty()) push_back(temp.pop_front());
    }

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
};

// 4. ПРИОРИТЕТНАЯ ОЧЕРЕДЬ (PriorityQueue) — Max-Heap
template <typename T>
class PriorityQueue {
private:
    T* heap;     
    int count;   
    int capacity; 

    void resize() {
        int newCapacity = capacity * 2;
        T* newHeap = new T[newCapacity];
        for (int i = 0; i < count; i++) newHeap[i] = heap[i];
        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;
    }

    // Всплытие элемента вверх (при добавлении)
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            // Если текущий больше родителя — меняем местами
            if (heap[index] > heap[parent]) {
                swap(heap[index], heap[parent]);
                index = parent;
            }
            else break;
        }
    }

    // Погружение элемента вниз (при извлечении)
    void heapifyDown(int index) {
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < count && heap[left] > heap[largest]) largest = left;
            if (right < count && heap[right] > heap[largest]) largest = right;

            if (largest == index) break;
            swap(heap[index], heap[largest]);
            index = largest;
        }
    }

public:
    PriorityQueue(int initialCapacity = 10) : capacity(initialCapacity), count(0) {
        heap = new T[capacity];
    }

    ~PriorityQueue() { delete[] heap; }

    // Добавление элемента
    void push(const T& value) {
        if (count >= capacity) resize();
        heap[count] = value;
        heapifyUp(count);
        count++;
    }

    // Извлечение максимального элемента
    T pop() {
        if (isEmpty()) throw underflow_error("Приоритетная очередь пуста");
        T maxValue = heap[0];
        heap[0] = heap[count - 1]; 
        count--;
        heapifyDown(0);            
        return maxValue;
    }

    // Просмотр максимального 
    T top() const {
        if (isEmpty()) throw underflow_error("Приоритетная очередь пуста");
        return heap[0];
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
};

int main() {
    setlocale(LC_ALL, "ru");

    cout << "ЛР №5: Структуры данных" << endl;

    cout << "\n1. Стек: удаление чётных" << endl;
    Stack<int> s;
    for (int i = 1; i <= 10; i++) s.push(i);
    cout << "Исходный стек: ";
    s.print();
    s.removeEven();
    cout << "После удаления чётных: ";
    s.print();

    
    cout << "\n2. Очередь: сортировка" << endl;
    Queue<int> q;
    q.enqueue(5);
    q.enqueue(3);
    q.enqueue(8);
    q.enqueue(1);
    cout << "Исходная очередь: ";
    q.print();
    q.sortQueue();
    cout << "После сортировки: ";
    q.print();

    cout << "\n3. Дек: удаление чётных" << endl;
    Deque<int> dq;
    for (int i = 1; i <= 8; i++) dq.push_back(i);
    cout << "Исходный дек: ";
    dq.print();
    dq.removeEven();
    cout << "После удаления чётных: ";
    dq.print();

    cout << "\n4. Приоритетная очередь: планировщик" << endl;
    PriorityQueue<int> pq;
    cout << "Добавляем процессы с приоритетами: 30, 10, 50, 20" << endl;
    pq.push(30);
    pq.push(10);
    pq.push(50);
    pq.push(20);
    cout << "Извлекаем процессы (от высшего приоритета к низшему): ";
    while (!pq.isEmpty()) {
        cout << pq.pop() << " ";
    }
    cout << endl;

    cout << "\n5. Проверка исключений" << endl;
    try {
        Stack<int> emptyStack;
        emptyStack.pop();
    }
    catch (const underflow_error& e) {
        cout << "Исключение: " << e.what() << endl;
    }

    return 0;
}