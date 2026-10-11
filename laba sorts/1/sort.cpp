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

int main() {
    ofstream f("optimization.csv");

if (!f.is_open()) {
    cerr << "Ошибка: не удалось открыть optimization.csv" << endl;
    return 1;
}

cout << "Файл открыт успешно" << endl;

    f << "N,time\n";

    for (int n = 100; n <= 10000; n += 100) {
        vector<int> a(n);

        for (int i = 0; i < n; i++) {
            a[i] = rand_uns(0, 1000000);
        }

        double start = get_time();

        bubble_sort(a);

        double time = get_time() - start;
        cout << a[0] << endl;
        f << n << "," << time << "\n";
    }

    f.close();

    return 0;
}