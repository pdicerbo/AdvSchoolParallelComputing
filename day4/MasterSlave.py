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
    # for inp in inputs:
    with open(inputs+'.out', 'w') as out_file:
        pid = Popen([executable, inputs], stdout=out_file)
        pid.wait()

def master():
    input_dir = os.path.abspath(sys.argv[2])

    inputs = input_files(input_dir)
    comm = MPI.COMM_WORLD
    size = comm.Get_size()
    MyStatus = MPI.Status()

    for MyFile in inputs:
        ReadyProc = comm.recv(source=MPI.ANY_SOURCE, tag=42, status=MyStatus)
        ToSend = [MyFile]
        comm.send(ToSend, dest=MyStatus.Get_source(), tag=24)

    # killing slaves
    for i in range(1,size):
        ReadyProc = comm.recv(source=MPI.ANY_SOURCE, tag=42, status=MyStatus)
        ToSend = [MyFile]
        comm.send(ToSend, dest=MyStatus.Get_source(), tag=0)
        # print("Killing process %d" % (MyStatus.Get_source()))

def slave():
    executable = os.path.abspath(sys.argv[1])
    comm = MPI.COMM_WORLD
    MyId = comm.Get_rank()
    MyStatus = MPI.Status()

    while True:
        MyList = [MyId]
        comm.send(MyList, dest=0, tag=42)
        MyList = comm.recv(source = 0, tag=MPI.ANY_TAG, status=MyStatus)
        if(MyStatus.Get_tag() == 0):
            break
        else:
            toprint = "Received "
            toprint += MyList[0]
            print(toprint)
            run(executable, MyList[0])

if __name__ == '__main__':

    comm = MPI.COMM_WORLD
    MyId = comm.Get_rank()

    if(MyId == 0):
        master()
    else:
        slave()
