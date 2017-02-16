#!/usr/bin/env python
"""
Serial program that runs a queue of serial simulations.
Usage: ./serial_queue.py <executable> <input_directory>
"""
import os
import sys
import glob
import time
from subprocess import Popen
from mpi4py import MPI

def input_files(my_path):
    return glob.glob(my_path+'/input*.txt')

def run(executable, inputs):
    for inp in inputs:
        with open(inp+'.out', 'w') as out_file:
           pid = Popen([executable, inp], stdout=out_file)
           pid.wait()

if __name__ == '__main__':
    executable = os.path.abspath(sys.argv[1])
    input_dir = os.path.abspath(sys.argv[2])

    comm = MPI.COMM_WORLD
    size = comm.Get_size()
    MyId = comm.Get_rank()
    # print("I am here: MyId=%d of %d" %(MyId, size))
    
    comm.Barrier()
    t1 = time.time()

    inputs = input_files(input_dir)
    
    nfiles = len(inputs)/size
    rest = 0
    if(MyId < len(inputs)%size):
        nfiles = nfiles + 1
    else:
        rest = len(inputs)%size

    # the output of sort function is not
    # guaranteed to be in order
    inputs.sort()
    
    """
    NICE ALTERNATIVE:
    local_inputs = []
    for i in range(MyId, len(inputs), size):
        local_inputs.append(inputs[i])
    """

    StartPoint = MyId*nfiles+rest
    EndPoint   = StartPoint + nfiles
    local_inputs = inputs[StartPoint:EndPoint]

    run(executable, local_inputs)
    
    t3 = time.time()
    comm.Barrier()
    t2 = time.time()

    print("Process %d has %d files and employes %f" % (MyId, len(local_inputs), t3-t1))
    
    if(MyId == 0):
        print("Elapsed time {:5.2f}".format(t2-t1))
    
