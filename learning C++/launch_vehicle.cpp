#include <iostream>
#include <cmath>
using namespace std;

const float dt  = 0.001f;
const float g0  = 9.81f;             // стандартное ускорение, м/с²
const float muz = 3.98600441e14f;    // гравитационный параметр Земли, м³/с²
const float Rz  = 6371000.0f;        // радиус Земли, м
const float Cx  = 0.3f;              // коэффициент обтекаемости

// плотность атмосферы на высоте h, кг/м³ (экспоненциальная модель)
float rho(float h) { return 1.225f * exp(-h / 7500.0f); }

// ускорение свободного падения на высоте h
float grav(float h) { return muz / ((Rz + h) * (Rz + h)); }

struct launch_vehicle {
    float F;   // тяга, Н
    float u;   // скорость истечения, м/с
    float M0;  // стартовая масса ступени со всем, что выше, кг
    float M1;  // масса в конце горения, кг
    float S;   // площадь миделя, м²
};

struct result {
    float h;    // высота, м
    float v;    // скорость, м/с
    float t;    // время горения, с
    float nx;   // максимальная осевая перегрузка, g
};

// Полёт по формуле Мещерского с потерями (из записки «А-том», п. 2.2):
// m dv/dt = mu*u − m g(h) sinθ − ½ Cx ρ(h) v² S
// θ — угол тангажа: вертикальный старт, затем плавный разворот к 25°
result calculation(launch_vehicle st, float h0, float v0) {
    float h = h0;
    float v = v0;
    float m = st.M0;
    float t = 0;
    float nxmax = 0;

    while (m > st.M1) {
        t += dt;
        m -= (st.F / st.u) * dt;   // расход массы mu = F/u

        float theta = 90.0f;
        if (h > 1000.0f)
            theta = max(25.0f, 90.0f - (h - 1000.0f) / 50000.0f * 65.0f);
        float sinT = sin(theta * 3.14159265f / 180.0f);

        float Faer = 0.5f * Cx * rho(h) * v * v * st.S;
        v += ((st.F - Faer) / m - grav(h) * sinT) * dt;
        h += v * sinT * dt;

        float nx = st.F / m / g0;
        if (nx > nxmax) nxmax = nx;
    }

    return {h, v, t, nxmax};
}

int main() {
    // РН «А-том»: три ступени (записка, таблицы 3 и 5 — «общепринятые формулы»)
    // Полезный груз 100 т входит в массу третьей ступени.
    const int n = 3;
    launch_vehicle stages[n] = {
        // связка 1+2: 8×РД-180, топливо только из первой ступени
        {2 * 15306219.0f, 3060.0f, 2685822.0f, 1075663.0f, 33.7f},
        // вторая ступень: 4×РД-180
        {15306219.0f,     3320.0f,  921014.0f,  373107.0f, 24.0f},
        // третья ступень: 6×РД-0150
        {3236195.0f,      4600.0f,  324854.0f,  130364.0f, 24.0f},
    };

    float h = 0, v = 0, t = 0;

    for (int i = 0; i < n; i++) {
        result r = calculation(stages[i], h, v);
        h = r.h;
        v = r.v;
        t += r.t;

        cout << "Ступень " << i + 1 << ": высота " << r.h
             << " м, скорость " << r.v << " м/с, время горения " << r.t
             << " с, макс. перегрузка " << r.nx << " g" << endl;
    }

    float vK = v;   // скорость в конце активного участка

    // после выгорания топлива — полёт по инерции вверх
    while (v > 0) {
        t += dt;
        v -= grav(h) * dt;
        h += v * dt;
    }

    cout << "Максимальная высота: " << h << " м" << endl;
    cout << "Скорость в конце активного участка: " << vK << " м/с" << endl;
    cout << "Общее время полёта: " << t << " с" << endl;

    return 0;
}
