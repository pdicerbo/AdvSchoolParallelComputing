Introduction to Python for HPC
==============================

### First run the single queue

unzip inputs.zip

module load gnu/6.1.0
gcc -o serial_program.x serial_program.c

qsub job_launcher.sh
