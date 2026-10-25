#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm>
#include <streambuf>
using namespace std;
using namespace chrono;

// Поток-приёмник: принимает символы, но никуда их не сохраняет.
class NullBuffer : public streambuf {
protected:
    int overflow(int c) override {
        return c;
    }
};

// Измеряет время выполнения функции в секундах.
template <typename Function>
double measure(Function function) {
    auto start = steady_clock::now();
    function();
    auto finish = steady_clock::now();

    return duration<double>(finish - start).count();
}

int main() {
    ofstream file("royal_battle.csv");
    file << "N,generation,forward,backward,output\n";

    mt19937 gen(42);
    uniform_int_distribution<int> dist(0, 1000000);

    const int repeats = 100;
    const int max_n = 50000;
    volatile long long checksum = 0;

    NullBuffer null_buffer;
    ostream sink(&null_buffer);

    for (int n = 1000; n <= max_n; n += 1000) {
        vector<int> a(n);

        // 1. Генерация массива случайных чисел.
        double generation = measure([&]() {
            for (int rep = 0; rep < repeats; rep++) {
                for (int i = 0; i < n; i++) {
                    a[i] = dist(gen);
                }
                checksum += a[rep % n];
            }
        }) / repeats;

        // Заполняем массив один раз перед измерением обработки.
        for (int i = 0; i < n; i++) {
            a[i] = dist(gen);
        }

        // 2. Арифметическая обработка от начала к концу.
        double forward = measure([&]() {
            for (int rep = 0; rep < repeats; rep++) {
                for (int i = 0; i < n; i++) {
                    a[i] = a[i] + 1;
                }
                checksum += a[rep % n];
            }
        }) / repeats;

        // 3. Арифметическая обработка от конца к началу.
        double backward = measure([&]() {
            for (int rep = 0; rep < repeats; rep++) {
                for (int i = n - 1; i >= 0; i--) {
                    a[i] = a[i] + 1;
                }
                checksum += a[rep % n];
            }
        }) / repeats;

        // 4. Формирование вывода всего массива.
        double output = measure([&]() {
            for (int i = 0; i < n; i++) {
                sink << a[i] << ' ';
            }
            sink << '\n';
        });

        file << n << ','
             << generation << ','
             << forward << ','
             << backward << ','
             << output << '\n';

        cerr << "N = " << n << " готово\n";
    }

    file.close();

    // Используем checksum, чтобы результаты вычислений оставались значимыми.
    cerr << "Контрольная сумма: " << checksum << '\n';
    return 0;
}