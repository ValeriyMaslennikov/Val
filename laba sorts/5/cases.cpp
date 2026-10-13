#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <random>
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

void selection_sort(vector<int>& a) {
    int n = a.size();
    for (int i = 0; i < n - 1; i++) {
        int mn = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[mn]) mn = j;
        }
        swap(a[i], a[mn]);
    }
}

void merge_sort(vector<int>& a, vector<int>& temp, int l, int r) {
    if (r - l <= 1) return;

    int m = (l + r) / 2;
    merge_sort(a, temp, l, m);
    merge_sort(a, temp, m, r);

    int i = l, j = m, k = l;
    while (i < m && j < r) {
        if (a[i] <= a[j]) temp[k++] = a[i++];
        else temp[k++] = a[j++];
    }
    while (i < m) temp[k++] = a[i++];
    while (j < r) temp[k++] = a[j++];

    for (int p = l; p < r; p++) a[p] = temp[p];
}

void quick_sort(vector<int>& a, int l, int r) {
    int i = l, j = r;
    int pivot = a[(l + r) / 2];

    while (i <= j) {
        while (a[i] < pivot) i++;
        while (a[j] > pivot) j--;
        if (i <= j) {
            swap(a[i], a[j]);
            i++;
            j--;
        }
    }

    if (l < j) quick_sort(a, l, j);
    if (i < r) quick_sort(a, i, r);
}

void heap_sort(vector<int>& a) {
    int n = a.size();

    for (int i = n / 2 - 1; i >= 0; i--) {
        int p = i;
        while (true) {
            int largest = p;
            int l = 2 * p + 1;
            int r = 2 * p + 2;

            if (l < n && a[l] > a[largest]) largest = l;
            if (r < n && a[r] > a[largest]) largest = r;
            if (largest == p) break;

            swap(a[p], a[largest]);
            p = largest;
        }
    }

    for (int i = n - 1; i > 0; i--) {
        swap(a[0], a[i]);

        int p = 0;
        while (true) {
            int largest = p;
            int l = 2 * p + 1;
            int r = 2 * p + 2;

            if (l < i && a[l] > a[largest]) largest = l;
            if (r < i && a[r] > a[largest]) largest = r;
            if (largest == p) break;

            swap(a[p], a[largest]);
            p = largest;
        }
    }
}

int main() {
    ofstream f("cases.csv");
    f << "N,case,bubble,insertion,selection,merge,quick,heap\n";

    mt19937 gen(12345);
    uniform_int_distribution<int> dist(0, 1000000);

    const int repeats = 3;

    for (int n = 100; n <= 3000; n += 100) {
        vector<int> random_array(n);
        for (int& x : random_array) x = dist(gen);

        vector<int> sorted_array = random_array;
        sort(sorted_array.begin(), sorted_array.end());

        vector<int> reversed_array = sorted_array;
        reverse(reversed_array.begin(), reversed_array.end());

        vector<pair<string, vector<int>>> cases = {
            {"best", sorted_array},
            {"average", random_array},
            {"worst", reversed_array}
        };

        for (auto& item : cases) {
            vector<int> original = item.second;
            double total[6] = {};

            for (int rep = 0; rep < repeats; rep++) {
                vector<int> a;

                a = original;
                double start = get_time();
                bubble_sort(a);
                total[0] += get_time() - start;

                a = original;
                start = get_time();
                insertion_sort(a);
                total[1] += get_time() - start;

                a = original;
                start = get_time();
                selection_sort(a);
                total[2] += get_time() - start;

                a = original;
                vector<int> temp(n);
                start = get_time();
                merge_sort(a, temp, 0, n);
                total[3] += get_time() - start;

                a = original;
                start = get_time();
                quick_sort(a, 0, n - 1);
                total[4] += get_time() - start;

                a = original;
                start = get_time();
                heap_sort(a);
                total[5] += get_time() - start;
            }

            f << n << "," << item.first;
            for (double t : total) f << "," << t / repeats;
            f << "\n";
        }

        cout << "N = " << n << " done\n";
    }

    f.close();
    cout << "Saved to cases.csv\n";
    return 0;
}