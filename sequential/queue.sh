#!/bin/bash

running=$(squeue -u ${USER} | egrep "bench_.*[R|Q]")
if [ -n "${running}" ] ;
then
    echo "------------------------------------"
    echo ${USER} has this job in the queue or running 
    squeue | grep ${USER}
    echo
    echo please delete this job or wait for it to complete
    exit 1
fi

sbatch --partition=Centaurus --time=02:00:00 --nodes=1 --ntasks-per-node=16 bench_sequential.sh
