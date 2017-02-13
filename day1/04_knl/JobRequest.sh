#!/bin/bash

#PBS -N FirstJob
#PBS -l walltime=00:00:50
#PBS -l select=1:ncpus=64:mpiprocs=256:mcdram=flat:mem=40GB
#PBS -A train_scA2017_0

module load autoload intelmpi

cd $PBS_O_WORKDIR

# export I_MPI_DEBUG=5

mpirun -np 64 ./a.out
