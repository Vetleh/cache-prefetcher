import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np

x = np.array([1, 2, 4, 8, 16])
y1 = np.array([1.01, 1.02, 1.02, 1.03, 0.96])  # width
y2 = np.array([1.01, 1.02, 1.04, 0.96, 0.92])  # depth
y3 = np.array([1.01, 1.02, 1.05, 1.03, 0.97])  # hybrid


ax = plt.subplot(111)

ax.plot(x, y1, x, y2, x, y3, linewidth="2",
        marker="o", markersize="5")

ax.spines['right'].set_visible(False)
ax.spines['top'].set_visible(False)

plt.xscale('log', base=2)
plt.xticks(x)
ax.get_xaxis().set_major_formatter(mpl.ticker.ScalarFormatter())

plt.ylabel("Average speedup")
plt.xlabel("Prefetcher degree")

plt.ylim(0.91, 1.06)

plt.legend(['G/DC (Width)', 'G/DC (Depth)', 'G/DC (Hybrid)'])

plt.grid()


plt.savefig('speedup.png', dpi=400)
plt.show()
