#!/bin/bash
rm -rf result.txt

#serial
#gcc dijkstra-original-randGraph.c -lm -lpthread

#pthread
#gcc dijkstra-pthread.c -lm -lpthread

#if openmpi without pthread
#mpicc dijkstra-bcast-nopthread.c -lm

#if openmpi+pthread
mpicc dijkstra-bcast.c -lm -lpthread

for (( i=24; i<=32024; i=i+1000 ))
do
  cnt=0
  for ((j=0; j<=10; j++))
  do
    #if pthread
    #run=$(./a.out $i)
    #if openmpi
    run=$( mpiexec -n 2 a.out $i )
    cnt=$(echo "${cnt}+${run}"|bc)
  done
  mean_avg=$(echo "${cnt}/10" | bc -l)
  mean_avg=$(echo ${mean_avg} | cut -b1-6)
  echo "$i $mean_avg"
  echo "$i $mean_avg" >> result.txt
done
