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
    static default_random_engine e(
        chrono::steady_clock::now().time_since_epoch().count()
    );
    uniform_int_distribution<int> d(min, max);
    return d(e);
}

// Пузырьковая сортировка
void bubble_sort(vector<int>& a) {
    int n = a.size();

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
            }
        }
    }
}

// Сортировка вставками
void insertion_sort(vector<int>& a) {
    int n = a.size();

    for (int i = 1; i < n; i++) {
        int x = a[i];
        int j = i - 1;

        while (j >= 0 && a[j] > x) {
            a[j + 1] = a[j];
            j--;
        }

        a[j + 1] = x;
    }
}

// Сортировка выбором
void selection_sort(vector<int>& a) {
    int n = a.size();

    for (int i = 0; i < n - 1; i++) {
        int min_pos = i;

        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[min_pos]) {
                min_pos = j;
            }
        }

        swap(a[i], a[min_pos]);
    }
}

int main() {
    ofstream f("sorts.csv");

    f << "N,bubble,insertion,selection\n";

    // Размеры массивов
    for (int n = 100; n <= 10000; n += 100) {
        vector<int> original(n);

        // Генерируем массив один раз,
        // чтобы генерация не попадала в измерение времени
        for (int i = 0; i < n; i++) {
            original[i] = rand_uns(0, 1000000);
        }

        vector<int> a;

        // Пузырьковая сортировка
        a = original;
        double start = get_time();
        bubble_sort(a);
        double bubble_time = get_time() - start;

        // Сортировка вставками
        a = original;
        start = get_time();
        insertion_sort(a);
        double insertion_time = get_time() - start;

        // Сортировка выбором
        a = original;
        start = get_time();
        selection_sort(a);
        double selection_time = get_time() - start;

        f << n << ","
          << bubble_time << ","
          << insertion_time << ","
          << selection_time << "\n";

        cout << n << " / "
             << bubble_time << " / "
             << insertion_time << " / "
             << selection_time << endl;
    }

    f.close();

    return 0;
}