#!/bin/bash
#PBS -N python_HPC
#PBS -A train_scA2017
#PBS -q R457635
###PBS -l select=1:ncpus=1:mpiprocs=1:mem=1Gb
#PBS -l select=2:ncpus=36:mpiprocs=36:mem=118Gb
#PBS -l walltime=00:01:00

cd $PBS_O_WORKDIR

module purge
module load python/2.7.12
module load gnu/6.1.0
module load openmpi/1-10.3--gnu--6.1.0

source env/bin/activate

rm -f inputs/*.out

#./single_queue.py serial_program.x inputs
mpirun ./multiple_queues.py serial_program.x inputs
#mpirun ./master_slave.py serial_program.x inputs
#./fault_tollerance.py mpi_program.x input.txt > out.txt 2> err.txt

deactivate
