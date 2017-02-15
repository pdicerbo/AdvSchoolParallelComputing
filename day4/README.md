Introduction to Python for HPC
==============================

### First run the single queue example

Extract the input files:

    unzip inputs.zip

Load the module of the GNU compiler and compule the serial code:

    module load gnu/6.1.0

    gcc -o serial\_program.x serial\_program.c

Launch the batch-script:

    qsub job\_launcher.sh

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

    mpirun ./multiple\_queues.py serial\_program.x inputs

    deactivate
    
Launch the batch-script:

    qsub job\_launcher.sh

### Exercise: master - slave

### Exercise: fault tollerance

