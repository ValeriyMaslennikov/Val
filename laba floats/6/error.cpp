#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    const int n = 1000000;
    float sum = 1.0f;

    for (int i = 0; i < n; i++)
        sum += 0.0000001f;

    cout << fixed << setprecision(10);
    cout << "Expected: " << 1.1 << '\n';
    cout << "float sum: " << sum << '\n';

    double sum_double = 1.0;
    for (int i = 0; i < n; i++)
        sum_double += 0.0000001;

    cout << "double sum: " << sum_double << '\n';

    return 0;
}
