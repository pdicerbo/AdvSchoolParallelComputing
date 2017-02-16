Introduction to Python for HPC
==============================

### First run the single queue example

Extract the input files:

    unzip inputs.zip

Load the module of the GNU compiler and compule the serial code:

    module load gnu/6.1.0
    gcc -o serial_program.x serial_program.c

Launch the batch-script:

    qsub job_launcher.sh

### Install mpi4py using Virtualenv and Pip

Load the module of python and the MPI library:

    module load python/2.7.12
    module load openmpi/1-10.3--gnu--6.1.0

Create and acrivate a virtual environment:

    virtualenv env
    source env/bin/activate
    
Install MPI4PY:

    pip install mpi4py

### Exercise: multiple queues

Edit the file multiple_queues.py as explained by the teacher.

In the file job_launcher.sh decomment the lines:

    #PBS -l select=2:ncpus=36:mpiprocs=36:mem=118Gb
    module load openmpi/1-10.3--gnu--6.1.0
    source env/bin/activate
    mpirun ./multiple_queues.py serial_program.x inputs
    deactivate
    
Launch the batch-script:

    qsub job_launcher.sh

### Exercise: master - slave

Edit the file master_slave.py as explained by the teacher.

In the file job_launcher.sh decomment the line:

    mpirun ./master_slave.py serial_program.x inputs
    
Launch the batch-script:

    qsub job_launcher.sh

### Exercise: fault tollerance

Compile the MPI code:

    mpicc -o mpi_programm.x mpi_program.x

Edit the file fault_tollerance.py as explained by the teacher.

In the file job_launcher.sh edit the select line:

    #PBS -l select=8:ncpus=36:mpiprocs=36:mem=118Gb
    
and deccoment the line:

    ./fault_tollerance.py mpi_program.x input.txt > out.txt 2> err.txt

Launch the batch-script:

    qsub job_launcher.sh


