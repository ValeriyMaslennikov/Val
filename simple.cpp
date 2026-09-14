#include <iostream>
using namespace std;

bool simple(int n){
	for (int i = 2; i < n; ++i)
	{
		if (n%i==0)
		{
			return false;
		}
	}
	return true;
}

int main(){
	int n;
	cin >> n;
	if (n==1)
	{
		cout <<2 <<endl;
		return 88;
	}
	int s = 1;
	int k = 1;
	while(s<n){
		k+=2;
		if (simple(k))
		{
			s++;
		}
	}



	cout <<k <<endl;
	return 88;
}

