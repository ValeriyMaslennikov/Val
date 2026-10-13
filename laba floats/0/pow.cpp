#include <iostream>
#include <cmath>
using namespace std;

unsigned long long int my_pow(unsigned long long int x,
                              unsigned long long int n) {
    unsigned long long int result = 1;
    for (unsigned long long int i = 0; i < n; i++) {
        result *= x;
    }
    return result;
}

int main() {
    unsigned long long int x = 3;

    cout << fixed;
    cout.precision(0);

    cout << "FLOAT:\n";
    cout << "n\tmy_pow\t\tstd::pow(float)\n";

    for (int n = 0; n <= 50; n++) {
        unsigned long long int exact = my_pow(x, n);
        float result = pow((float)x, (float)n);

        cout << n << "\t" << exact << "\t\t"
             << result << '\n';
    }

    cout << "\nDOUBLE:\n";
    cout << "n\tmy_pow\t\tstd::pow(double)\n";

    for (int n = 0; n <= 50; n++) {
        unsigned long long int exact = my_pow(x, n);
        double result = pow((double)x, (double)n);

        cout << n << "\t" << exact << "\t\t"
             << result << '\n';
    }

    return 0;
}
