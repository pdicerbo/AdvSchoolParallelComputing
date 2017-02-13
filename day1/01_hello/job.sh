#!/bin/bash

#-------------------- hw section -------------------------------

#### Ask for a number of chunks (select=) each with cpus "ncpus" 
#PBS -l 

#-------------------- queue system -------------------------------

#### Queue of submission (comment if not using the reservation)
#PBS -q

#### Maximum length of the job (hh:mm:ss)
#PBS -l 

# --------------- accounting/budget  -----------------------------

#### account number (type saldo -b)
#PBS -A 

# ---------------- other info ------------------------------------

#### File for standard output and error
#PBS -o job.out
#PBS -e job.err

#### Job name
#PBS -N hello_my_friend

#### send email to the following address
#PBS -M 

#### send email after abort or end
#PBS -m 

# -----------end of PBS keywords section -------------------------

hostname 

echo 'hello world' 

sleep 4





