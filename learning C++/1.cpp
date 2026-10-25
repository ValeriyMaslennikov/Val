#include <iostream>
using namespace std;

int main() {
    unsigned int a;
    if (!(cin >> a)) return 0;

    for (int i = 31; i >= 0; --i) {
        if(i>0 && i<31 && (i+1)%4 == 0){
            cout << ' ';
            if(i%8==0) cout<<' ';
        }
        cout << ((a >> i) & 1);
    }

    return 0;
}
