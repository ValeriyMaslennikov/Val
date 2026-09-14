#include <iostream>
using namespace std;

int main(){
	int Vv;// - максимальная скорость Вовы.
	int Vm; //- скорость киберпса.
	int L; //- начальное расстояние от киберпса до Вовы.
	int K;// - начальное расстояние от Вовы до забора.
	int N;// - начальное количество мешков у Вовы.

	cin >> Vv;
	cin >> Vm;
	cin >> L;
	cin >> K;
	cin >> N;

	int Xm = 0;
	int Xv = L;
	int Xb = L+K;
	int Vt = Vv - N;

	while(Xm<=Xb+Vm){
		if (N>0)
		{
			N--;
			Vt++;
		}
		Xm+=Vm;
		Xv+=Vt;
		if (Xv>Xb && Xm<=Xb || Xv >= Xb && Xm<Xb)
		{
			cout << N;
			return 0;
		}
		if (Xm>=Xv)
		{
			cout << 0;
			return 0;
		}
	}

	return 0;
}