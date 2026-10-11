#include <iostream>
using namespace std;

void printrotate(char m[100][100], int col, int row){
    for (int j = col - 1; j >= 0; --j)
    {
        for (int i = 0; i < row; ++i)
        {
            cout << m[i][j];
        }
        cout << '\n';
    }
}

int main(){
    int n, m;
    cin >> n >> m;

    char numbers[100][100];

    cin.get(); // забираем '\n' после m

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cin.get(numbers[i][j]);
        }
        cin.get(); // забираем '\n' после строки
    }

    printrotate(numbers, m, n);

    return 0;
}