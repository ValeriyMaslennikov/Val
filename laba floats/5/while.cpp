#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    float x = 0.0f;
    unsigned long long int iterations = 0;

    while (true) {
        float next = x + 1.0f;

        if (next == x) {
            cout << fixed << setprecision(1);
            cout << "Infinite loop at x = " << x << '\n';
            cout << "Iterations = " << iterations << '\n';
            break;
        }

        x = next;
        iterations++;
    }

    return 0;
}