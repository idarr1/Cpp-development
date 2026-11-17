#include <iostream>
#include <chrono>
#include <algorithm>
#include <windows.h>

using namespace std;
using namespace chrono;

// 1. БЫСТРАЯ СОРТИРОВКА (Quick Sort)
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortRec(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortRec(arr, low, pi - 1);
        quickSortRec(arr, pi + 1, high);
    }
}

void quickSort(int arr[], int n) {
    quickSortRec(arr, 0, n - 1);
}

// 2. СОРТИРОВКА СЛИЯНИЕМ (Merge Sort)
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSortRec(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortRec(arr, left, mid);
        mergeSortRec(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void mergeSort(int arr[], int n) {
    mergeSortRec(arr, 0, n - 1);
}

// 3. ПИРАМИДАЛЬНАЯ СОРТИРОВКА (Heap Sort)
void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// 4. TIM SORT
const int RUN = 32;

void insertionSortTim(int arr[], int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void mergeTim(int arr[], int l, int m, int r) {
    int len1 = m - l + 1, len2 = r - m;
    int* left = new int[len1];
    int* right = new int[len2];
    for (int i = 0; i < len1; i++) left[i] = arr[l + i];
    for (int i = 0; i < len2; i++) right[i] = arr[m + 1 + i];

    int i = 0, j = 0, k = l;
    while (i < len1 && j < len2) {
        if (left[i] <= right[j]) arr[k++] = left[i++];
        else arr[k++] = right[j++];
    }
    while (i < len1) arr[k++] = left[i++];
    while (j < len2) arr[k++] = right[j++];
    delete[] left; delete[] right;
}

void timSort(int arr[], int n) {
    for (int i = 0; i < n; i += RUN)
        insertionSortTim(arr, i, min((i + RUN - 1), (n - 1)));

    for (int size = RUN; size < n; size = 2 * size) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = min((left + 2 * size - 1), (n - 1));
            if (mid < right) mergeTim(arr, left, mid, right);
        }
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

    cout << "ЛР №7: Эффективные сортировки\n";

    const int N = 10000;
    int* arr = new int[N];
    for (int i = 0; i < N; i++) arr[i] = rand() % N;

    cout << "\nСравнение на массиве из " << N << " элементов (мкс):\n";
    cout << "Quick Sort:  " << measureTime(quickSort, arr, N) << endl;
    cout << "Merge Sort:  " << measureTime(mergeSort, arr, N) << endl;
    cout << "Heap Sort:   " << measureTime(heapSort, arr, N) << endl;
    cout << "Tim Sort:    " << measureTime(timSort, arr, N) << endl;

    delete[] arr;
    return 0;
}