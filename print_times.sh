#!/bin/bash
for (( i=10; i<=40000; i=i+1000 ))
do
    ./a.out $i >> result.txt
done
