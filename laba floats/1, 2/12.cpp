#include <iostream>
#include <iomanip>
using namespace std;

void print_binary(unsigned int x) {
    for (int i = 31; i >= 0; i--)
        cout << ((x >> i) & 1u);
    cout << '\n';
}

union FloatBits {
    float f;
    unsigned int u;
};

int main() {
    unsigned int x = 3;
    cout << "unsigned int: ";
    print_binary(x);

    FloatBits number;
    number.f = 13.25f;

    cout << "float:        ";
    print_binary(number.u);

    cout << "Sign:     " << ((number.u >> 31) & 1u) << '\n';
    cout << "Exponent: " << ((number.u >> 23) & 255u) << '\n';
    cout << "Mantissa: " << (number.u & 0x7FFFFFu) << '\n';

    return 0;
}
