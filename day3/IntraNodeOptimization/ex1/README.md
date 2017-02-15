EX1 : Performance Characterization
==================================

Env :

    module load autoload vtune mkl


ex1.1
------

Use Makefile to compile all version of MM software and verify the time to solution using launcher.sh .
With VTune Profiler and Opt report check HPC performance of matrixAVX.x and matrixNoVec.x.
Compare the performance of 3 neested loop MM with BLAS MM.


ex1.2
------

Use Makefile to compile the code and analyze the performance with Intel VTUNE. 
In the code it is present an error, fix it. 
Try to change the datatype of array B. What is the datatype that produces the best performance?

 
ex1.3
------
Use Makefile to compile the code and analyze the performance with Intel VTUNE. 
Can you explain the profiling result?
