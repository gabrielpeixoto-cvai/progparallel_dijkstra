#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt

fig = plt.figure()

ax1 = fig.add_subplot(111)

ax1.set_title("Dijkstra Serial Execution")
ax1.set_xlabel('Number of Vertices')
ax1.set_ylabel('Time (ms)')

plt.plot(*np.loadtxt("data_files/data_serialtp.dat",unpack=True), linewidth=2.0,label='serial')

#pthread

if 1:
    plt.plot(*np.loadtxt("data_files/data_pthread2tp.dat",unpack=True), linewidth=2.0, label='pthread2')
    plt.plot(*np.loadtxt("data_files/data_pthread4tp.dat",unpack=True), linewidth=2.0, label='pthread4')
    plt.plot(*np.loadtxt("data_files/data_pthread8tp.dat",unpack=True), linewidth=2.0, label='pthread8')

#mpi
if 0:
    plt.plot(*np.loadtxt("data_files/data_mpi2tp.dat",unpack=True), linewidth=2.0, label='mpi 2')
    plt.plot(*np.loadtxt("data_files/data_mpi4tp.dat",unpack=True), linewidth=2.0, label='mpi 4')
    plt.plot(*np.loadtxt("data_files/data_mpi8tp.dat",unpack=True), linewidth=2.0, label='mpi 8')

#mpi+pthread
if 0:
    plt.plot(*np.loadtxt("data_files/data_mpit2_4tp.dat",unpack=True), linewidth=2.0, label='mpi 2 + pthread 4')
    plt.plot(*np.loadtxt("data_files/data_mpit2_2tp.dat",unpack=True), linewidth=2.0, label='mpi 2 + pthread 2')
    #plt.plot(*np.loadtxt("data_mpt8tp.dat",unpack=True), linewidth=2.0, label='mpi 8 + pthread')

leg = ax1.legend(loc=2)

plt.show()
