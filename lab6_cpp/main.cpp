#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <windows.h>

using namespace std;
using namespace chrono;

// ЧАСТЬ 1: РЕКУРСИВНЫЕ АЛГОРИТМЫ

// 1. Факториал
long long factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// 2. Треугольное число (T_n = 1 + 2 + ... + n)
int triangular(int n) {
    if (n == 1) return 1;
    return n + triangular(n - 1);
}

// 3. Произведение цифр числа
int productOfDigits(int n) {
    if (n < 10) return n;
    return (n % 10) * productOfDigits(n / 10);
}

// 4. Перебор всех перестановок
void generatePermutations(vector<int>& arr, int start, int end) {
    if (start == end) {
        for (int x : arr) cout << x << " ";
        cout << endl;
        return;
    }
    for (int i = start; i <= end; i++) {
        swap(arr[start], arr[i]);
        generatePermutations(arr, start + 1, end);
        swap(arr[start], arr[i]); 
    }
}

// ЧАСТЬ 2: ПРОСТЫЕ СОРТИРОВКИ

// 1. Пузырьковая сортировка
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// 2. Сортировка вставками
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// 3. Сортировка выбором
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) minIndex = j;
        }
        if (minIndex != i) swap(arr[i], arr[minIndex]);
    }
}

// ИЗМЕРЕНИЕ ВРЕМЕНИ
long long measureTime(void (*sortFunc)(int[], int), int arr[], int n) {
    int* copy = new int[n];
    for (int i = 0; i < n; i++) copy[i] = arr[i];
    auto start = high_resolution_clock::now();
    sortFunc(copy, n);
    auto end = high_resolution_clock::now();
    delete[] copy;
    return duration_cast<microseconds>(end - start).count();
}

int main() {
    setlocale(LC_ALL, "ru");

    cout << "ЛР №6: Рекурсия и сортировки\n";

    cout << "\n1. Рекурсивные алгоритмы\n";
    cout << "Факториал 5: " << factorial(5) << endl;
    cout << "Треугольное число 5: " << triangular(5) << endl;
    cout << "Произведение цифр 1234: " << productOfDigits(1234) << endl;

    cout << "Перестановки [1, 2, 3]:" << endl;
    vector<int> perm = { 1, 2, 3 };
    generatePermutations(perm, 0, perm.size() - 1);

    cout << "\n2. Простые сортировки\n";
    const int N = 5000;
    int* randomArr = new int[N];
    int* sortedArr = new int[N];
    int* reverseArr = new int[N];

    for (int i = 0; i < N; i++) {
        randomArr[i] = rand() % N;
        sortedArr[i] = i;
        reverseArr[i] = N - i;
    }

    cout << "Сравнение на массиве из " << N << " элементов (мкс):\n";
    cout << "Тип массива\t\tПузырьковая\tВставками\tВыбором\n";

    cout << "Случайный:\t\t"
        << measureTime(bubbleSort, randomArr, N) << "\t\t"
        << measureTime(insertionSort, randomArr, N) << "\t\t"
        << measureTime(selectionSort, randomArr, N) << endl;

    cout << "Почти отсортированный:\t"
        << measureTime(bubbleSort, sortedArr, N) << "\t\t"
        << measureTime(insertionSort, sortedArr, N) << "\t\t"
        << measureTime(selectionSort, sortedArr, N) << endl;

    cout << "Обратно отсортированный:\t"
        << measureTime(bubbleSort, reverseArr, N) << "\t\t"
        << measureTime(insertionSort, reverseArr, N) << "\t\t"
        << measureTime(selectionSort, reverseArr, N) << endl;

    delete[] randomArr;
    delete[] sortedArr;
    delete[] reverseArr;

    return 0;
}