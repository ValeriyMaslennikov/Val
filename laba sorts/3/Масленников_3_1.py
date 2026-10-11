
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

# Папка, в которой лежит сам plot.py
ROOT = Path(__file__).resolve().parent

# Пути к результатам предыдущих заданий
DATA_0 = ROOT / "0" / "sorts.csv"
DATA_1 = ROOT / "1"
DATA_2 = ROOT / "2" / "fast_sorts.csv"

# Сортировки O(N^2)
slow = pd.read_csv(DATA_0)

# Быстрые сортировки O(N log N)
fast = pd.read_csv(DATA_2)

# Соответствие алгоритмов из задания 0
# алгоритм -> столбец с измерениями
slow_columns = {
    "Bubble": "bubble",
    "Insertion": "insertion",
    "Selection": "selection"
}

# Файлы оптимизаций из задания 1
optimization_files = {
    "Без флага": DATA_1 / "none.csv",
    "-O0": DATA_1 / "O0.csv",
    "-O1": DATA_1 / "O1.csv",
    "-O2": DATA_1 / "O2.csv",
    "-O3": DATA_1 / "O3.csv"
}

# Для удобства приводим времена к общему размеру массива.
# Интерполяция нужна, если размеры массивов в заданиях
# 0 и 2 различаются.
def interpolate_time(df, column, sizes):
    return np.interp(
        sizes,
        df["N"].to_numpy(),
        df[column].to_numpy()
    )

# Общие размеры массива — размеры из задания 0,
# на которых строились три простые сортировки.
sizes = slow["N"].to_numpy()

# Читаем измерения из задания 1.
opt_data = {}
for name, filename in optimization_files.items():
    if filename.exists():
        opt_data[name] = pd.read_csv(filename)

# Четыре варианта оптимизации: без флага, O0, O1, O2, O3.
# Для сравнения с быстрыми сортировками выбираем O2.
# Без флага и O0 часто дают близкие результаты,
# но это разные варианты компиляции.
optimization_names = list(opt_data.keys())

# Графики:
# 1. Без флага
# 2. -O0
# 3. -O1
# 4. -O2
# 5. -O3
# На каждом: три медленные сортировки и три быстрые.
# Для каждого варианта оптимизации быстрые сортировки
# нужно измерить отдельно, поэтому пока строим графики
# для медленных сортировок и показываем быстрые при -O2.

# Быстрые сортировки сравниваются с простыми при -O2.
fast_columns = {
    "Merge": "merge",
    "Quick": "quick",
    "Heap": "heap"
}

# Таблица ускорения для трёх быстрых алгоритмов
# относительно трёх простых алгоритмов.
# Ускорение = время медленной / время быстрой.
# Берём общие размеры массива.
common_sizes = sizes[
    (sizes >= fast["N"].min()) &
    (sizes <= fast["N"].max())
]

slow_common = slow[slow["N"].isin(common_sizes)]
fast_common = fast[fast["N"].isin(common_sizes)]

rows = []


# Оставляем только размеры N, которые есть в обоих файлах
common = pd.merge(slow, fast, on="N", how="inner")

rows = []

for _, row in common.iterrows():
    n = int(row["N"])

    for slow_name, slow_col in slow_columns.items():
        for fast_name, fast_col in fast_columns.items():
            rows.append({
                "N": n,
                "Slow sort": slow_name,
                "Fast sort": fast_name,
                "Speedup": row[slow_col] / max(row[fast_col], 1e-12)
            })

speedups = pd.DataFrame(rows)
speedups.to_csv(ROOT / "speedups.csv", index=False)

summary = speedups.groupby(
    ["Slow sort", "Fast sort"]
)["Speedup"].mean().unstack()

summary.to_csv(ROOT / "speedups_summary.csv")

print("Средние ускорения:")
print(summary.round(2))

speedups = pd.DataFrame(rows)
speedups.to_csv(ROOT / "speedups.csv", index=False)

# Среднее ускорение для каждой пары алгоритмов
summary = speedups.groupby(
    ["Slow sort", "Fast sort"]
)["Speedup"].mean().unstack()

summary.to_csv(ROOT / "speedups_summary.csv")

print("Средние ускорения:")
print(summary.round(2))

# Для каждого доступного уровня оптимизации строим график.
# Три медленные сортировки берём из задания 0.
# Три быстрые сортировки берём из задания 2.
# Важно: чтобы сравнение было корректным, fast_sorts.cpp
# нужно компилировать с тем же флагом, что и sort.cpp.
for opt_name, opt_df in opt_data.items():
    plt.figure(figsize=(10, 6))

    # Три простые сортировки
    for name, col in slow_columns.items():
        plt.plot(slow["N"], slow[col], label=name)

    # Три быстрые сортировки
    for name, col in fast_columns.items():
        plt.plot(fast["N"], fast[col], label=name)

    plt.xlabel("Размер массива N")
    plt.ylabel("Время, с")
    plt.title(f"Сравнение сортировок ({opt_name})")
    plt.legend()
    plt.grid(True)

    safe_name = opt_name.replace("-", "").replace(" ", "_")
    plt.savefig(ROOT / f"comparison_{safe_name}.png", dpi=200)
    plt.close()

print("Графики и таблицы сохранены в общей папке.")