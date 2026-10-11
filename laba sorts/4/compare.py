
import csv
import random
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent

def bubble_sort(a):
    a = a.copy()
    n = len(a)

    for i in range(n - 1):
        for j in range(n - i - 1):
            if a[j] > a[j + 1]:
                a[j], a[j + 1] = a[j + 1], a[j]

    return a

def insertion_sort(a):
    a = a.copy()

    for i in range(1, len(a)):
        x = a[i]
        j = i - 1

        while j >= 0 and a[j] > x:
            a[j + 1] = a[j]
            j -= 1

        a[j + 1] = x

    return a

def selection_sort(a):
    a = a.copy()
    n = len(a)

    for i in range(n - 1):
        min_pos = i

        for j in range(i + 1, n):
            if a[j] < a[min_pos]:
                min_pos = j

        a[i], a[min_pos] = a[min_pos], a[i]

    return a

sorts = {
    "bubble": bubble_sort,
    "insertion": insertion_sort,
    "selection": selection_sort
}

with open(ROOT / "python_sorts.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["N", "bubble", "insertion", "selection"])

    for n in range(100, 5001, 100):
        original = [random.randint(0, 1_000_000) for _ in range(n)]
        times = []

        for sort_func in sorts.values():
            start = time.perf_counter()
            result = sort_func(original)
            elapsed = time.perf_counter() - start

            assert result == sorted(original)
            times.append(elapsed)

        writer.writerow([n] + times)
        print(f"N = {n}: {times}")

print("Результаты Python сохранены.")
