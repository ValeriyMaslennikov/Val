import pandas as pd
import matplotlib.pyplot as plt

files = {
    "Без оптимизации": "none.csv",
    "-O0": "O0.csv",
    "-O1": "O1.csv",
    "-O2": "O2.csv",
    "-O3": "O3.csv"
}

plt.figure()

for name, filename in files.items():
    data = pd.read_csv(filename)
    plt.plot(data["N"], data["time"], label=name)

plt.xlabel("Размер массива N")
plt.ylabel("Время, с")
plt.title("Влияние оптимизации компилятора на Bubble Sort")
plt.legend()
plt.grid()

plt.savefig("optimization.png")
plt.show()