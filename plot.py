#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt

fig = plt.figure()

ax1 = fig.add_subplot(111)

ax1.set_title("Dijkstra Serial Execution")
ax1.set_xlabel('Number of Vertices')
ax1.set_ylabel('Time (ms)')

plt.plot(*np.loadtxt("result.txt",unpack=True), linewidth=2.0)#, label='serial')

leg = ax1.legend()

plt.show()
