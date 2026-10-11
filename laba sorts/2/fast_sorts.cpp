
#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <vector>

using namespace std;

double get_time() {
    return chrono::duration<double>(
        chrono::steady_clock::now().time_since_epoch()
    ).count();
}

int rand_uns(int min, int max) {
    static mt19937 gen(
        chrono::steady_clock::now().time_since_epoch().count()
    );
    uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

// Сортировка слиянием
void merge_sort(vector<int>& a, vector<int>& temp, int left, int right) {
    if (right - left <= 1) return;

    int mid = left + (right - left) / 2;

    merge_sort(a, temp, left, mid);
    merge_sort(a, temp, mid, right);

    int i = left, j = mid, k = left;

    while (i < mid && j < right) {
        if (a[i] <= a[j]) temp[k++] = a[i++];
        else temp[k++] = a[j++];
    }

    while (i < mid) temp[k++] = a[i++];
    while (j < right) temp[k++] = a[j++];

    for (int p = left; p < right; p++) {
        a[p] = temp[p];
    }
}

// Быстрая сортировка Хоара
void quick_sort(vector<int>& a, int left, int right) {
    int i = left, j = right;
    int pivot = a[left + (right - left) / 2];

    while (i <= j) {
        while (a[i] < pivot) i++;
        while (a[j] > pivot) j--;

        if (i <= j) {
            swap(a[i], a[j]);
            i++;
            j--;
        }
    }

    if (left < j) quick_sort(a, left, j);
    if (i < right) quick_sort(a, i, right);
}

// Пирамидальная сортировка
void sift_down(vector<int>& a, int n, int i) {
    while (true) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && a[left] > a[largest]) {
            largest = left;
        }

        if (right < n && a[right] > a[largest]) {
            largest = right;
        }

        if (largest == i) break;

        swap(a[i], a[largest]);
        i = largest;
    }
}

void heap_sort(vector<int>& a) {
    int n = a.size();

    for (int i = n / 2 - 1; i >= 0; i--) {
        sift_down(a, n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(a[0], a[i]);
        sift_down(a, i, 0);
    }
}

int main() {
    ofstream f("fast_sorts.csv");

    if (!f.is_open()) {
        cerr << "Не удалось создать fast_sorts.csv" << endl;
        return 1;
    }

    f << "N,merge,quick,heap\n";

    for (int n = 1000; n <= 50000; n += 1000) {
        vector<int> original(n);

        for (int i = 0; i < n; i++) {
            original[i] = rand_uns(0, 1000000);
        }

        vector<int> a = original;
        vector<int> temp(n);

        double start = get_time();
        merge_sort(a, temp, 0, n);
        double merge_time = get_time() - start;

        a = original;
        start = get_time();
        quick_sort(a, 0, n - 1);
        double quick_time = get_time() - start;

        a = original;
        start = get_time();
        heap_sort(a);
        double heap_time = get_time() - start;

        f << n << "," << merge_time << ","
          << quick_time << "," << heap_time << "\n";

        cout << "N = " << n << " готово" << endl;
    }

    f.close();
    cout << "Результаты сохранены в fast_sorts.csv" << endl;

    return 0;
}