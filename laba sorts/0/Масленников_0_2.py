import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("sorts.csv")

# Обычный масштаб
plt.figure()

plt.plot(data["N"], data["bubble"], label="Bubble sort")
plt.plot(data["N"], data["insertion"], label="Insertion sort")
plt.plot(data["N"], data["selection"], label="Selection sort")

plt.xlabel("Размер массива N")
plt.ylabel("Время, с")
plt.title("Сравнение сортировок O(N²)")
plt.legend()
plt.grid()

plt.savefig("sorts.png")
plt.show()


# Двойной логарифмический масштаб
plt.figure()

plt.loglog(data["N"], data["bubble"], label="Bubble sort")
plt.loglog(data["N"], data["insertion"], label="Insertion sort")
plt.loglog(data["N"], data["selection"], label="Selection sort")

plt.xlabel("Размер массива N")
plt.ylabel("Время, с")
plt.title("Сравнение сортировок O(N²), log-log")
plt.legend()
plt.grid()

plt.savefig("sorts_loglog.png")
plt.show()