# importing package
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np

# create data
x = np.arange(10)
y = [1.08, 1.06, 1.05, 1.05, 1.04, 1.02, 1.01, 1.01, 1.00, 1.00]
# width = 0.2

ax = plt.subplot(111)
ax.bar(x, y, zorder=3, color=['tab:gray', 'tab:gray', 'tab:green',
       'tab:gray', 'tab:orange', 'tab:blue', 'tab:gray', 'tab:gray', 'tab:gray', 'tab:gray'])

ax.spines['right'].set_visible(False)
ax.spines['top'].set_visible(False)


plt.ylabel("Average speedup")
plt.ylim(1.00, 1.10)

plt.xticks(x, ['DCPT-P', 'RPT', 'G/DC (Hybrid)', 'DCPT',
           'G/DC (Depth)', 'G/DC(Width)', 'Sequential on access', 'Tagged', 'Sequential on miss', 'None'], rotation=45, ha='right')

plt.grid(axis='y', zorder=0)
plt.tight_layout()


blue = mpatches.Patch(color='tab:blue', label='G/DC (Width)')
orange = mpatches.Patch(color='tab:orange', label='G/DC (Depth)')
green = mpatches.Patch(color='tab:green', label='G/DC (Hybrid)')
plt.legend(handles=[blue, orange, green])

plt.savefig('comparison.png', dpi=400)
plt.show()
