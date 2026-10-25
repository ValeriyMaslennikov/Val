import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("royal_battle.csv")

columns = {
    "generation": "Генерация массива",
    "forward": "Обработка слева направо",
    "backward": "Обработка справа налево",
    "output": "Формирование вывода"
}

for column, label in columns.items():
    plt.plot(data["N"], data[column], label=label)

plt.xlabel("Размер массива N")
plt.ylabel("Среднее время, с")
plt.title("Королевская битва")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("royal_battle.png", dpi=200)
plt.show()