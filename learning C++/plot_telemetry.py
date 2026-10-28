# Графики полёта ракеты-носителя по телеметрии launch_vehicle.cpp
# Запускается автоматически из программы (или вручную: python plot_telemetry.py)
# Зависимости: matplotlib  (pip install matplotlib)

import sys

try:
    import matplotlib.pyplot as plt
except ImportError:
    print("Нужна библиотека matplotlib. Установите её:  pip install matplotlib")
    sys.exit(1)

# --- чтение telemetry.csv (разделитель ';') ---
names = ("t", "h", "v", "m", "q", "nx", "theta", "faer", "stage")
cols = {name: [] for name in names}

try:
    f = open("telemetry.csv", encoding="utf-8")
except FileNotFoundError:
    print("Файл telemetry.csv не найден — сначала запустите launch_vehicle")
    sys.exit(1)

with f:
    f.readline()  # строка заголовка
    for line in f:
        p = line.strip().split(";")
        if len(p) < len(names):
            continue
        for name, val in zip(names, map(float, p)):
            cols[name].append(val)

if not cols["t"]:
    print("telemetry.csv пуст — данных для графиков нет")
    sys.exit(1)

# --- моменты отделения ступеней ---
cuts = [cols["t"][i] for i in range(1, len(cols["stage"]))
        if cols["stage"][i] != cols["stage"][i - 1]]

t = cols["t"]

fig, ax = plt.subplots(2, 3, figsize=(15, 8))
fig.suptitle("Полёт ракеты-носителя (расчёт launch_vehicle)", fontsize=14)


def style(a, title, ylabel):
    a.set_title(title)
    a.set_ylabel(ylabel)
    a.grid(True, alpha=0.3)
    for c in cuts:
        a.axvline(c, color="gray", ls="--", lw=0.8, alpha=0.7)


# 1) высота над уровнем моря
style(ax[0][0], "Высота над уровнем моря", "h, км")
ax[0][0].plot(t, [x / 1000 for x in cols["h"]])

# 2) скорость
style(ax[0][1], "Скорость", "v, м/с")
ax[0][1].plot(t, cols["v"])

# 3) масса (моё дополнение)
style(ax[0][2], "Масса", "m, т")
ax[0][2].plot(t, [x / 1000 for x in cols["m"]])

# 4) скоростной напор (моё дополнение)
style(ax[1][0], "Скоростной напор", "q, кПа")
ax[1][0].plot(t, [x / 1000 for x in cols["q"]])

# 5) перегрузка (моё дополнение): предел для экипажа 4.5 g из записки «А-том»
style(ax[1][1], "Перегрузка", "nx, g")
ax[1][1].plot(t, cols["nx"])
ax[1][1].axhline(4.5, color="red", ls=":", lw=1.2)
ax[1][1].text(t[-1], 4.5, " предел 4.5 g", color="red", va="bottom", ha="right", fontsize=9)

# 6) угол тангажа (моё дополнение)
style(ax[1][2], "Угол тангажа", "θ, град")
ax[1][2].plot(t, cols["theta"])
ax[1][2].set_ylim(-5, 95)

for a in ax[1]:
    a.set_xlabel("Время, с")

fig.tight_layout()
fig.savefig("telemetry.png", dpi=150)
print("График сохранён в telemetry.png")
plt.show()
