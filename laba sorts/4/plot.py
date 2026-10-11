
import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

ROOT = Path(__file__).resolve().parent

py = pd.read_csv(ROOT / "python_sorts.csv")
cpp = pd.read_csv(ROOT / "cpp_sorts.csv")

sorts = {
    "bubble": "Пузырьковая сортировка",
    "insertion": "Сортировка вставками",
    "selection": "Сортировка выбором"
}

for col, title in sorts.items():
    plt.figure(figsize=(9, 5))

    plt.plot(cpp["N"], cpp[col], label="C++ (-O2)")
    plt.plot(py["N"], py[col], label="Python")

    plt.xlabel("Размер массива N")
    plt.ylabel("Время, с")
    plt.title(title + ": C++ против Python")
    plt.legend()
    plt.grid(True)

    plt.savefig(ROOT / f"{col}_comparison.png", dpi=200)
    plt.close()

print("Три графика сохранены в папке 4.")
