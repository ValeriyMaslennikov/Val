#include <iostream>
using namespace std;

const float dt = 0.001f;
const float g  = 9.81f;

struct launch_vehicle {
    float u;   // скорость истечения, м/с
    float mu;  // расход массы, кг/с
    float M0;  // стартовая масса, кг
    float M1;  // сухая масса, кг
};

struct result {
    float h;   // высота, м
    float v;   // скорость, м/с
    float t;   // время работы двигателя, с
};

result calculation(launch_vehicle stage, float h0, float v0) {
    float u  = stage.u;
    float mu = stage.mu;
    float M0 = stage.M0;
    float M1 = stage.M1;

    float h = h0;
    float v = v0;
    float m = M0;
    float t = 0;

    // Горим, пока масса больше сухой
    while (m > M1) {
        t += dt;
        m -= mu * dt;
        v += (u * mu * dt) / m - g * dt;   // тяга минус гравитация
        h += v * dt;
    }

    return {h, v, t};
}

int main() {
	bool again = false;
	do
	{
    float u, mu, M0, M1;
    cout << "Введите u mu M0 M1 через пробел: ";
    cin >> u >> mu >> M0 >> M1;

    launch_vehicle booster = {u, mu, M0, M1};
    result final = calculation(booster, 0, 0);

    cout << "Высота: " << final.h << " м" << endl;
    cout << "Скорость: " << final.v << " м/с" << endl;
    cout << "Время горения: " << final.t << " с" << endl;

    
    cout << "Запустить ещё раз? (1/0): ";
    cin >> again;

	} while (again);
    return 0;
}
