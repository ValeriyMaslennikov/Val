import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

ROOT = Path(__file__).resolve().parent
df = pd.read_csv(ROOT / "cases.csv")

algorithms = {
    "bubble": "Bubble sort",
    "insertion": "Insertion sort",
    "selection": "Selection sort",
    "merge": "Merge sort",
    "quick": "Quick sort",
    "heap": "Heap sort"
}

cases = {
    "best": "Отсортированный массив",
    "average": "Случайный массив",
    "worst": "Массив в обратном порядке"
}

for algorithm, name in algorithms.items():
    plt.figure(figsize=(9, 6))

    for case, label in cases.items():
        data = df[df["case"] == case]
        plt.plot(data["N"], data[algorithm], marker="o",
                 markersize=3, label=label)

    plt.title(name)
    plt.xlabel("Размер массива N")
    plt.ylabel("Время сортировки, с")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig(ROOT / f"{algorithm}.png", dpi=200)
    plt.close()

print("Графики сохранены.")