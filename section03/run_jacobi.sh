#!/bin/bash
#PBS -q dssc 
#PBS -l nodes=2:ppn=2 
#PBS -l walltime=0:10:00
cd $PBS_O_WORKDIR
module load openmpi-4.1.1+gnu-9.3.0

mpirun -np 24 ./jacobi3D.x < input.txt > output.txt