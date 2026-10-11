import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

data = pd.read_csv("fast_sorts.csv")

# График времени работы
plt.figure()

plt.plot(data["N"], data["merge"], label="Merge Sort")
plt.plot(data["N"], data["quick"], label="Quick Sort")
plt.plot(data["N"], data["heap"], label="Heap Sort")

plt.xlabel("Размер массива N")
plt.ylabel("Время, с")
plt.title("Сравнение быстрых сортировок")
plt.legend()
plt.grid()
plt.savefig("fast_sorts.png", dpi=200)
plt.show()

# Проверка сложности O(N log N)
plt.figure()

for name in ["merge", "quick", "heap"]:
    plt.plot(
        data["N"],
        data[name] / (data["N"] * np.log(data["N"])),
        label=name
    )

plt.xlabel("Размер массива N")
plt.ylabel("t / (N ln N)")
plt.title("Проверка сложности O(N log N)")
plt.legend()
plt.grid()
plt.savefig("fast_sorts_complexity.png", dpi=200)
plt.show()