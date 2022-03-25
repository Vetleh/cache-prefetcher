# importing package
import matplotlib.pyplot as plt
import numpy as np

# create data
x = np.arange(12)
y1 = [1.00, 1.13, 1.09, 1.09, 1.03, 1.04, 1.02,
      1.00, 0.97, 0.97, 0.97, 1.00]  # width
y2 = [1.17, 1.15, 1.11, 1.09, 1.04, 1.04, 1.02,
      1.00, 0.96, 0.96, 0.97, 0.99]  # depth
y3 = [1.37, 1.15, 1.08, 1.08, 1.04, 1.04, 1.02,
      1.00, 0.98, 0.98, 0.96, 0.95]  # hybrid
width = 0.2

# plot data in grouped manner of bar type
ax = plt.subplot(111)

ax.bar(x-width, y1, width, zorder=3)
ax.bar(x, y2, width, zorder=3)
ax.bar(x+width, y3, width, zorder=3)

ax.spines['right'].set_visible(False)
ax.spines['top'].set_visible(False)

plt.ylabel("Speedup")
plt.ylim(0.90, 1.40)

plt.xticks(x, ['ammp', 'wupwise', 'galgel', 'applu', 'apsi', 'bzip2_program',
           'bzip2_graphic', 'bzip2_source', 'art470', 'art110', 'twolf', 'swim'], rotation=45, ha='right')

plt.legend(['G/DC (Width)', 'G/DC (Depth)', 'G/DC (Hybrid)'])

plt.grid(axis='y', zorder=0)
plt.tight_layout()

plt.savefig('tests.png', dpi=400)
plt.show()
