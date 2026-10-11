#include <iostream>
using namespace std;

int main(){
    int n;
    cin >> n;
    int numbers[1000] = {0};
    int res[1000];
    for(int i = 0; i<n; i++){
        int curEl;
        cin >> curEl;
        
        if(i==0){
            numbers[i]=curEl;
            continue;
        }
        bool flag = false;
        for(int j = 0; j<i; j++){
            if(curEl > numbers[j]){
                for(int k = i-1; k>=j; k--){
                    numbers[k+1] = numbers[k];
                }
                numbers[j] = curEl;
                flag = true;
                break;
            }
        }
        if (!flag)
        {
            numbers[i] = curEl;
        }   
    }
    int m;
    cin >> m;
    for(int i = 0; i<m; i++){
        cout<< numbers[i] << ' ';
    }
    
    
    return 88;
}