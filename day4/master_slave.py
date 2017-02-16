#!/usr/bin/env python

import os
import sys
import glob
import time
from subprocess import Popen
from mpi4py import MPI

WORKTAG = 0
DIETAG = 1

def input_file(my_path):
    return glob.glob(my_path+'/input*.txt')

def run(executable, inputs):
    for inp in inputs:
        with open(inp+'.out', 'w') as out_file:
           pid = Popen([executable, inp], stdout=out_file)
           pid.wait()

def master(inputs):
    for i in range(1,size):
        inp = inputs.pop()
        comm.send(inp, dest=i, tag=WORKTAG)

    while inputs:
        im_free = comm.recv(source=MPI.ANY_SOURCE, tag=10)
        inp = inputs.pop()
        comm.send(inp, dest=im_free, tag=WORKTAG)

    for i in range(1,size):
        im_free = comm.recv(source=i, tag=10)
        comm.send(obj=None, dest=i, tag=DIETAG)


def slave(executable):
    c = 0
    while True:
        status = MPI.Status()
        inp = comm.recv(source=0, tag=MPI.ANY_TAG, status=status)
        if status.Get_tag(): break
        run(executable, [inp])
        c += 1
        comm.send(rank, dest=0, tag=10)
    return c

    
if __name__ == '__main__':
    executable = os.path.abspath(sys.argv[1])
    input_dir = os.path.abspath(sys.argv[2])
    
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()

    comm.Barrier()
    t1 = time.time()

    if rank == 0:
        inputs = input_file(input_dir)
        print("Number of elaborations: {:d}".format(len(inputs)))
        master(inputs)
    else:
        c = slave(executable)
        t3 = time.time()
        print("I am rank {:d}, elapsed time {:5.2f}, executed {:d} elaborations.".format(rank, t3-t1, c))

    comm.Barrier()
    t2 = time.time()
    if rank == 0:
        print("Elapsed time {:5.2f}".format(t2-t1))
