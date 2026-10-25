#include <iostream>
#include <limits>
using namespace std;

union FloatBits {
    float f;
    unsigned int u;
};

union DoubleBits {
    double d;
    unsigned long long int u;
};

void print_binary(unsigned int x) {
    for (int i = 31; i >= 0; i--)
        cout << ((x >> i) & 1u);
    cout << '\n';
}

void print_binary64(unsigned long long int x) {
    for (int i = 63; i >= 0; i--)
        cout << ((x >> i) & 1ull);
    cout << '\n';
}

int main() {
    FloatBits f;

    f.f = numeric_limits<float>::infinity();
    cout << "float +inf: ";
    print_binary(f.u);

    f.f = -numeric_limits<float>::infinity();
    cout << "float -inf: ";
    print_binary(f.u);

    f.f = numeric_limits<float>::quiet_NaN();
    cout << "float nan:  ";
    print_binary(f.u);

    DoubleBits d;

    d.d = numeric_limits<double>::infinity();
    cout << "double +inf: ";
    print_binary64(d.u);

    d.d = -numeric_limits<double>::infinity();
    cout << "double -inf: ";
    print_binary64(d.u);

    d.d = numeric_limits<double>::quiet_NaN();
    cout << "double nan:  ";
    print_binary64(d.u);

    return 0;
}