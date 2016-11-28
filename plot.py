#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt

fig = plt.figure()

ax1 = fig.add_subplot(111)

ax1.set_title("Dijkstra Serial Execution")
ax1.set_xlabel('Number of Vertices')
ax1.set_ylabel('Time (ms)')

plt.plot(*np.loadtxt("data_serial.dat",unpack=True), 'b', linewidth=2.0,label='serial')
plt.plot(*np.loadtxt("result.txt",unpack=True), 'g', linewidth=2.0,label='serial_mean')
#plt.plot(*np.loadtxt("data_pthread2.dat",unpack=True), 'g', linewidth=2.0, label='pthread2')
#plt.plot(*np.loadtxt("data_pthread4.dat",unpack=True), 'r', linewidth=2.0, label='pthread4')
#plt.plot(*np.loadtxt("data_pthread8.dat",unpack=True), 'k', linewidth=2.0, label='pthread4')

leg = ax1.legend()

plt.show()
