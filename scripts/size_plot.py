import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np

x = np.array([2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048])
y1 = np.array([0.97, 0.99, 1.01, 1.02, 1.02, 1.01, 1.02,
              1.02, 1.02, 1.02, 1.02])  # width
y2 = np.array([0.97, 1.01, 1.01, 1.03, 1.03, 1.04, 1.03,
              1.04, 1.03, 1.04, 1.04])  # depth
y3 = np.array([0.97, 1.02, 1.03, 1.04, 1.04, 1.04, 1.04,
              1.05, 1.04, 1.04, 1.04])  # hybrid

ax = plt.subplot(111)

ax.plot(x, y1, x, y2, x, y3, linewidth="2",
        marker="o", markersize="5")

ax.spines['right'].set_visible(False)
ax.spines['top'].set_visible(False)

plt.ylabel("Average speedup")
plt.xlabel("GHB and IT size")

plt.xscale('log', base=2)
plt.xticks(x)
ax.get_xaxis().set_major_formatter(mpl.ticker.ScalarFormatter())

plt.legend(['G/DC (Width)', 'G/DC (Depth)', 'G/DC (Hybrid)'])

plt.grid()


plt.savefig('size.png', dpi=400)
plt.show()
