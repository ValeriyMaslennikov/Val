#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdlib>

using namespace std;

const double dt  = 0.001;           // шаг интегрирования, с
const double g0  = 9.81;            // стандартное ускорение, м/с²
const double muz = 3.98600441e14;   // гравитационный параметр Земли, м³/с²
const double Rz  = 6371000.0;       // радиус Земли, м
const double p0  = 101325.0;        // давление на уровне моря, Па
const double Cx  = 0.3;             // коэффициент обтекаемости
const double PI  = 3.14159265358979;

double rho(double h)   { return 1.225 * exp(-h / 7500.0); }    // плотность атмосферы, кг/м³
double press(double h) { return p0 * exp(-h / 7500.0); }       // давление атмосферы, Па
double grav(double h)  { return muz / ((Rz + h) * (Rz + h)); } // g на высоте h, м/с²

struct stage {
    string name;
    double F;        // тяга всех двигателей ступени, Н
    double uSea;     // скорость истечения у земли, м/с
    double uVac;     // скорость истечения в вакууме, м/с
    double MT;       // масса топлива ступени, кг
    double MK;       // сухая масса ступени (конструкция и двигатели, без топлива и ступеней выше), кг
    double S;        // площадь миделя, м²
    double thetaEnd; // угол тангажа в конце работы ступени, град (90 — вертикально)
    double M0, M1;   // стартовая масса (со всем, что выше) и масса в конце горения, кг
};

struct result {
    double h;    // высота в конце горения, м
    double v;    // скорость в конце горения, м/с
    double t;    // время горения, с
    double nx;   // максимальная осевая перегрузка, g
};

// Скорость истечения растёт с высотой: от uSea у земли до uVac в вакууме,
// пропорционально падению давления атмосферы (таблица 2 записки)
double exhaustSpeed(const stage& st, double h) {
    double pr = press(h) / p0;   // 1 у земли, почти 0 в космосе
    return st.uVac - (st.uVac - st.uSea) * pr;
}

// Ввод числа: Enter — значение по умолчанию, иначе число из диапазона
double ask(const string& prompt, double def, double minV, double maxV) {
    for (;;) {
        cout << prompt << " [" << def << "]: ";
        string line;
        getline(cin, line);
        if (line.empty()) return def;

        char* end = nullptr;
        double val = strtod(line.c_str(), &end);
        if (end != line.c_str() && *end == '\0' && val >= minV && val <= maxV)
            return val;

        cout << "  Ошибка: нужно число от " << minV << " до " << maxV << ". Повторите." << endl;
    }
}

stage inputStage(int i, const stage& def) {
    stage s;
    cout << "\n--- СТУПЕНЬ " << i << " ---" << endl;
    cout << "Название [" << def.name << "]: ";
    getline(cin, s.name);
    if (s.name.empty()) s.name = def.name;

    s.F        = ask("  Тяга всех двигателей, Н", def.F, 1.0, 1e9);
    s.uSea     = ask("  Скорость истечения у земли, м/с", def.uSea, 1.0, 10000.0);
    s.uVac     = ask("  Скорость истечения в вакууме, м/с (не меньше, чем у земли)", def.uVac, s.uSea, 10000.0);
    s.MT       = ask("  Масса топлива, кг", def.MT, 1.0, 1e8);
    s.MK       = ask("  Сухая масса ступени (конструкция и двигатели), кг", def.MK, 0.0, 1e8);
    s.S        = ask("  Площадь миделя, м2", def.S, 0.1, 1e4);
    s.thetaEnd = ask("  Угол тангажа в конце работы, град (90 — вертикально, 0 — горизонтально)",
                     def.thetaEnd, 0.0, 90.0);
    return s;
}

// Горение одной ступени: уравнение Мещерского с потерями (записка, п. 2.2):
// m dv/dt = F − ½Cxρv²S − m g(h) sinθ,  dh/dt = v sinθ
// Разворот: вертикально до 1 км, затем линейно от thetaStart к st.thetaEnd
result calculation(const stage& st, double h0, double v0, double thetaStart,
                   double& t, ofstream& telem, int stageNo,
                   double& qmax, double& tQmax, double& hQmax) {
    double h = h0, v = v0, m = st.M0, ts = 0, nxmax = 0;
    double uAvg  = 0.5 * (st.uSea + st.uVac);
    double Tburn = st.MT / (st.F / uAvg);   // оценка времени горения для программы разворота

    long step = 0;
    while (m > st.M1) {
        t += dt;
        ts += dt;
        m -= st.F / exhaustSpeed(st, h) * dt;

        double theta = thetaStart + (st.thetaEnd - thetaStart) * min(1.0, ts / Tburn);
        if (h < 1000.0) theta = 90.0;
        double sinT = sin(theta * PI / 180.0);

        double q    = 0.5 * rho(h) * v * v;        // скоростной напор, Па
        double Faer = Cx * q * st.S;

        v += ((st.F - Faer) / m - grav(h) * sinT) * dt;
        h += v * sinT * dt;

        double nx = st.F / m / g0;
        if (nx > nxmax) nxmax = nx;
        if (q > qmax) { qmax = q; tQmax = t; hQmax = h; }

        if (step % 500 == 0)   // строка телеметрии каждые 0.5 с
            telem << t << ';' << h << ';' << v << ';' << m << ';' << q << ';'
                  << nx << ';' << theta << ';' << Faer << ';' << stageNo << '\n';
        step++;
    }
    telem << t << ';' << h << ';' << v << ';' << m << ';'
          << 0.5 * rho(h) * v * v << ';' << st.F / m / g0 << ';' << st.thetaEnd << ';'
          << Cx * 0.5 * rho(h) * v * v * st.S << ';' << stageNo << '\n';

    return {h, v, ts, nxmax};
}

// РН «А-том» по умолчанию: таблицы 2, 3 и 5 записки
// (топливо из таблицы 5, сухая масса = МК таблицы 5 минус всё, что выше ступени)
int main() {
    const stage atom[3] = {
        {"Первая (8хРД-180, связка)", 2 * 15306219.0, 3060.0, 3320.0, 1610159.0, 154649.0, 33.7, 45.0},
        {"Вторая (4хРД-180)",         15306219.0,     3060.0, 3320.0, 547907.0,  48253.0,  24.0, 25.0},
        {"Третья (6хРД-0150)",        3236195.0,      4600.0, 4600.0, 194490.0,  30364.0,  24.0, 0.0},
    };

    cout << "=== Баллистический расчёт многоступенчатой ракеты-носителя ===" << endl;
    cout << "Модель: уравнение Мещерского с гравитационными и аэродинамическими" << endl;
    cout << "потерями. Везде можно нажать Enter — подставится РН «А-том»." << endl;

    bool again = false;
    do {
        int n = (int)ask("\nСколько ступеней?", 3, 1, 10);
        double payload = ask("Масса полезной нагрузки, кг", 100000.0, 0.0, 1e7);

        stage stages[10];
        for (int i = 0; i < n; i++)
            stages[i] = inputStage(i + 1, atom[i < 3 ? i : 2]);

        // Сборка масс: верхняя ступень несёт полезный груз, каждая нижняя — всё, что выше
        double top = payload;
        for (int i = n - 1; i >= 0; i--) {
            stages[i].M1 = top + stages[i].MK;
            stages[i].M0 = stages[i].M1 + stages[i].MT;
            top = stages[i].M0;
        }

        // --- проверка данных перед расчётом ---
        double Mstart = stages[0].M0;
        double twr = stages[0].F / (Mstart * g0);
        cout << fixed << setprecision(2);
        cout << "\nСтартовая масса: " << setprecision(1) << Mstart / 1000.0 << " т" << endl;
        cout << "Тяговооружённость на старте: " << setprecision(2) << twr;
        if (twr <= 1.0) {
            cout << "  <- ОШИБКА: тяга меньше веса, ракета не оторвётся от стола." << endl;
            cout << "Увеличьте тягу первой ступени или уменьшите массы." << endl;
        } else {
            cout << (twr < 1.2 ? "  <- предупреждение: взлёт будет очень вялым." : "  - норма (обычно 1.2...1.5)") << endl;

            cout << "\nКонфигурация:" << endl;
            cout << "Ступень                              | Тяга, Н     | M0, т   | M1, т   | n0   | угол кон." << endl;
            for (int i = 0; i < n; i++) {
                cout << left << setw(36) << stages[i].name << " | "
                     << right << setw(11) << (long long)stages[i].F << " | "
                     << setw(7) << setprecision(1) << stages[i].M0 / 1000.0 << " | "
                     << setw(7) << stages[i].M1 / 1000.0 << " | "
                     << setprecision(2) << setw(4) << stages[i].F / (stages[i].M0 * g0) << " | "
                     << setprecision(0) << stages[i].thetaEnd << " град" << endl;
            }

            // --- расчёт ---
            ofstream telem("telemetry.csv");
            telem << "t,с;h,м;v,м/с;m,кг;q,Па;nx,g;тангаж,град;Fаэр,Н;ступень" << endl;

            double t = 0, h = 0, v = 0, theta = 90.0;
            double qmax = 0, tQmax = 0, hQmax = 0;

            cout << "\nРезультаты по ступеням:" << endl;
            cout << "Ступень            | t гор, с | dV факт | dV Циолк | макс. nx | h кон, км" << endl;

            for (int i = 0; i < n; i++) {
                double vBefore = v;
                result r = calculation(stages[i], h, v, theta, t, telem, i + 1, qmax, tQmax, hQmax);
                theta = stages[i].thetaEnd;
                h = r.h;
                v = r.v;

                double dVideal = 0.5 * (stages[i].uSea + stages[i].uVac) * log(stages[i].M0 / stages[i].M1);
                cout << left << setw(18) << stages[i].name.substr(0, 18) << " | "
                     << right << setprecision(1) << setw(8) << r.t << " | "
                     << setw(7) << setprecision(0) << v - vBefore << " | "
                     << setw(8) << dVideal << " | "
                     << setw(8) << setprecision(2) << r.nx << " | "
                     << setw(9) << setprecision(1) << r.h / 1000.0 << endl;
            }

            // --- полёт по инерции после выключения двигателя ---
            double hActive = h, vActive = v;
            double vy = v * sin(theta * PI / 180.0);   // вертикальная составляющая скорости
            while (vy > 0) {
                t += dt;
                vy -= grav(h) * dt;
                h += vy * dt;
            }

            // --- вывод итогов ---
            double rOrb = Rz + hActive;
            double vOrb = sqrt(muz / rOrb);        // первая космическая на высоте выключения
            double vEsc = sqrt(2.0) * vOrb;        // вторая космическая (отлёт к Марсу)

            cout << "\nИТОГИ:" << endl;
            cout << "Двигатель выключен: скорость " << setprecision(0) << vActive
                 << " м/с на высоте " << setprecision(1) << hActive / 1000.0 << " км" << endl;
            cout << "Первая космическая здесь: " << vOrb << " м/с, вторая: " << vEsc << " м/с" << endl;
            if (vActive >= vEsc)
                cout << "Достигнута вторая космическая скорость — возможен отлёт к Марсу!" << endl;
            else if (vActive >= vOrb)
                cout << "Орбитальная скорость достигнута (запас " << vActive - vOrb
                     << " м/с), до отлёта к Марсу не хватает " << vEsc - vActive << " м/с." << endl;
            else
                cout << "До орбитальной скорости не хватает " << vOrb - vActive
                     << " м/с, до отлёта к Марсу — " << vEsc - vActive << " м/с." << endl;

            cout << "Апогей: " << setprecision(1) << h / 1000.0 << " км";
            if (theta < 10.0)
                cout << " (траектория пологая, апогей ~= высоте выключения двигателя)";
            cout << endl;
            cout << "Максимальный скоростной напор: " << setprecision(1) << qmax / 1000.0
                 << " кПа на " << setprecision(1) << tQmax << " с (высота "
                 << hQmax / 1000.0 << " км)" << endl;
            cout << "Общее время полёта: " << setprecision(1) << t << " с" << endl;
            cout << "Телеметрия записана в telemetry.csv (разделитель ';')" << endl;

            // Построение графиков: запуск python-скрипта plot_telemetry.py
#ifdef _WIN32
            if (system("python plot_telemetry.py") != 0 &&
                system("py plot_telemetry.py") != 0)
#else
            if (system("python3 plot_telemetry.py") != 0 &&
                system("python plot_telemetry.py") != 0)
#endif
            {
                cout << "Не удалось запустить python." << endl;
                cout << "Установите python и matplotlib (pip install matplotlib)," << endl;
                cout << "затем графики можно построить вручную: python plot_telemetry.py" << endl;
            }
        }

        cout << "\nПовторить расчёт? (1/0): ";
        string line;
        getline(cin, line);
        again = (line == "1" || line == "да" || line == "y");
    } while (again);

    return 0;
}
