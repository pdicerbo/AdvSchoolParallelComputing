#!/bin/bash

#PBS -N FirstJob
#PBS -l walltime=00:03:00
#PBS -l select=1:ncpus=1:mem=128GB
#PBS -m abe
#PBS -M pierluigi.dicerbo@gmail.com
#PBS -q R457635
#PBS -A train_scA2017

hostname

echo 'hello world'

sleep 60
