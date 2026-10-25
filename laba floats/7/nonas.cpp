#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    float a = 1e20f;
    float b = -1e20f;
    float c = 1.77f;

    float result1 = (a + b) + c;
    float result2 = a + (b + c);

    cout << fixed << setprecision(2);
    cout << "(a + b) + c = " << result1 << '\n';
    cout << "a + (b + c) = " << result2 << '\n';

    return 0;
}