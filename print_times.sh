#!/bin/bash
rm -rf result.txt

#gcc dijkstra-original-randGraph.c -lm -lpthread
#if pthread
#gcc dijkstra-original-randGraph.c -lm -lpthread

#if openmpi
mpicc dijkstra-bcast-nopthread.c -lm -lpthread


for (( i=10; i<=40000; i=i+1000 ))
do
  cnt=0
  for ((j=0; j<=10; j++))
  do
    #if pthread
    #run=$(./a.out $i)
    #if openmpi
    run=$( mpiexec -n 4 a.out $i )
    cnt=$(echo "${cnt}+${run}"|bc)
  done
  mean_avg=$(echo "${cnt}/10" | bc -l)
  mean_avg=$(echo ${mean_avg} | cut -b1-6)
  echo "$i $mean_avg"
  echo "$i $mean_avg" >> result.txt
done
