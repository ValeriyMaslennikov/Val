#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
using namespace std;

void print_binary(unsigned int x) {
    for (int i = 31; i >= 0; i--)
        cout << ((x >> i) & 1u);
}

void print_binary64(unsigned long long int x) {
    for (int i = 63; i >= 0; i--)
        cout << ((x >> i) & 1ull);
}

union FloatBits {
    float f;
    unsigned int u;
};

union DoubleBits {
    double d;
    unsigned long long int u;
};

unsigned long long int my_pow(unsigned long long int x,
                              unsigned long long int n) {
    unsigned long long int result = 1;
    for (unsigned long long int i = 0; i < n; i++)
        result *= x;
    return result;
}

int main() {
    const unsigned long long int base = 3;
    int powers[] = {10, 15, 16, 20, 30, 33, 34, 40, 50, 60, 70, 80, 90, 100};

    cout << "FLOAT\n";
    cout << "n\tmy_pow\t\tstd::pow(float)\tbits\n";

    for (int n : powers) {
        FloatBits value;
        value.f = pow((float)base, (float)n);

        cout << n << '\t'
             << my_pow(base, n) << '\t'
             << setprecision(9) << defaultfloat << value.f
             << '\t';
        print_binary(value.u);
        cout << '\n';
    }

    cout << "\nDOUBLE\n";
    cout << "n\tmy_pow\t\tstd::pow(double)\tbits\n";

    for (int n : powers) {
        DoubleBits value;
        value.d = pow((double)base, (double)n);

        cout << n << '\t'
             << my_pow(base, n) << '\t'
             << setprecision(17) << defaultfloat << value.d
             << '\t';
        print_binary64(value.u);
        cout << '\n';
    }

    return 0;
}