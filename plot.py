#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt

fig = plt.figure()

ax1 = fig.add_subplot(111)

#ax1.set_title("Dijkstra Serial Execution")
ax1.set_xlabel('Number of Vertices')
ax1.set_ylabel('Time (ms)')

#plt.plot(*np.loadtxt("data_files/data_serialtp.dat",unpack=True), linewidth=2.0,label='serial')
plt.plot(*np.loadtxt("data_serial_cx_o2.dat",unpack=True), linewidth=2.0,label='serial')

#pthread

if 0:
    ax1.set_title("Dijkstra Serial X Pthread")
    plt.plot(*np.loadtxt("data_pthread2.dat",unpack=True), linewidth=2.0, label='pthread2')
    plt.plot(*np.loadtxt("data_pthread4.dat",unpack=True), linewidth=2.0, label='pthread4')
    plt.plot(*np.loadtxt("data_pthread8.dat",unpack=True), linewidth=2.0, label='pthread8')

#mpi
if 1:
    ax1.set_title("Dijkstra Serial X MPI")
    plt.plot(*np.loadtxt("data_mpi2.dat",unpack=True), linewidth=2.0, label='mpi 2')
    plt.plot(*np.loadtxt("data_mpi4.dat",unpack=True), linewidth=2.0, label='mpi 4')
    plt.plot(*np.loadtxt("data_mpi8.dat",unpack=True), linewidth=2.0, label='mpi 8')
    #plt.plot(*np.loadtxt("data_files/data_mpi2tp.dat",unpack=True), linewidth=2.0, label='mpi 2')
    #plt.plot(*np.loadtxt("data_files/data_mpi4tp.dat",unpack=True), linewidth=2.0, label='mpi 2')
    #plt.plot(*np.loadtxt("data_files/data_mpi8tp.dat",unpack=True), linewidth=2.0, label='mpi 2')

#mpi+pthread
if 0:
    ax1.set_title("Dijkstra Serial X MPI + Pthread")
    plt.plot(*np.loadtxt("data_mpi2pth2.dat",unpack=True), linewidth=2.0, label='mpi 2 + pthread 4')
    plt.plot(*np.loadtxt("data_mpi2pth4.dat",unpack=True), linewidth=2.0, label='mpi 2 + pthread 2')
    #plt.plot(*np.loadtxt("data_mpt8tp.dat",unpack=True), linewidth=2.0, label='mpi 8 + pthread')

if 0:
    ax1.set_title("Comparativo dos melhores resultados")
    plt.plot(*np.loadtxt("data_pthread4.dat",unpack=True), linewidth=2.0, label='pthread4')
    plt.plot(*np.loadtxt("data_mpi2.dat",unpack=True), linewidth=2.0, label='mpi 2')
    plt.plot(*np.loadtxt("data_mpi2pth4.dat",unpack=True), linewidth=2.0, label='mpi 2 + pthread 2')

#pthread grafo conexo + -O2
if 0:
    ax1.set_title("Dijkstra Serial X Pthread com -O2")
    plt.plot(*np.loadtxt("data_pthread2_cx_O2.dat",unpack=True), linewidth=2.0, label='pthread2_cx_o2')
    plt.plot(*np.loadtxt("data_pthread4_cx_O2.dat",unpack=True), linewidth=2.0, label='pthread4_cx_o2')
    plt.plot(*np.loadtxt("data_pthread8_cx_O2.dat",unpack=True), linewidth=2.0, label='pthread8_cx_o2')

#mpi
if 0:
    ax1.set_title("Dijkstra Serial X MPI com -O2")
    plt.plot(*np.loadtxt("data_mpi2_cx_O2.dat",unpack=True), linewidth=2.0, label='mpi 2')
    plt.plot(*np.loadtxt("data_mpi4_cx_O2.dat",unpack=True), linewidth=2.0, label='mpi 4')
    plt.plot(*np.loadtxt("data_mpi8_cx_O2.dat",unpack=True), linewidth=2.0, label='mpi 8')

#mpi+pthread
if 0:
    ax1.set_title("Dijkstra Serial X MPI + Pthread com -O2")
    plt.plot(*np.loadtxt("data_mpi2pth2_cx_O2.dat",unpack=True), linewidth=2.0, label='mpi 2 + pthread 2')
    plt.plot(*np.loadtxt("data_mpi2pth4_cx_O2.dat",unpack=True), linewidth=2.0, label='mpi 2 + pthread 4')
    #plt.plot(*np.loadtxt("data_mpt8tp.dat",unpack=True), linewidth=2.0, label='mpi 8 + pthread')

if 0:
    ax1.set_title("Comparativo dos melhores resultados com -O2")
    plt.plot(*np.loadtxt("data_pthread4_cx_O2.dat",unpack=True), linewidth=2.0, label='pthread4_cx_o2')
    plt.plot(*np.loadtxt("data_mpi2_cx_O2.dat",unpack=True), linewidth=2.0, label='mpi 2')
    plt.plot(*np.loadtxt("data_mpi2pth2_cx_O2.dat",unpack=True), linewidth=2.0, label='mpi 2 + pthread 2')


leg = ax1.legend(loc=2)

plt.show()
