
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;

double get_time() {
    return chrono::duration<double>(
        chrono::steady_clock::now().time_since_epoch()
    ).count();
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

void insertion_sort(vector<int>& a) {
    for (int i = 1; i < (int)a.size(); i++) {
        int x = a[i];
        int j = i - 1;

        while (j >= 0 && a[j] > x) {
            a[j + 1] = a[j];
            j--;
        }

        a[j + 1] = x;
    }
}

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
    ofstream f("cpp_sorts.csv");

    if (!f.is_open()) {
        cerr << "Не удалось создать cpp_sorts.csv" << endl;
        return 1;
    }

    mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dist(0, 1000000);

    f << "N,bubble,insertion,selection\n";

    for (int n = 100; n <= 5000; n += 100) {
        vector<int> original(n);

        for (int& x : original) {
            x = dist(gen);
        }

        vector<int> a = original;

        double start = get_time();
        bubble_sort(a);
        double t_bubble = get_time() - start;

        a = original;
        start = get_time();
        insertion_sort(a);
        double t_insertion = get_time() - start;

        a = original;
        start = get_time();
        selection_sort(a);
        double t_selection = get_time() - start;

        f << n << "," << t_bubble << ","
          << t_insertion << "," << t_selection << "\n";

        cout << "N = " << n << " готово" << endl;
    }

    f.close();
    cout << "Результаты сохранены в cpp_sorts.csv" << endl;

    return 0;
}
