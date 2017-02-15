3d vector update:
================

Run the program
----------------

We provide you with a Fortran source code that performs a 3d 
vector update. Can you tell why the omp loop is applied to 
the innermost loop?
 
Compile the code with the makefile provided. Try different 
placing policies and take note of the timings. Try to explain 
the results with Vtune (General Exploration and Memory collection)     

Is there any false sharing? Why?

Improved solution
-----------------

Try to improve the code by tackling the main performance issues. 

Hint:
* in the solution provided, for a given set of k,j, divides the i domain
(worksharing do loop) and update the matrix [1]. Then j,k are incremented,
domain i is divided and the matrix updated [2]. Note that update [1] 
and [2] are independent.
    

* think of a different solution where each thread works on different
i domains, but simulateneously for all the k,j values. In other words,
as long as each thread computes mat([is,ie],j,k) for any given k,j 
we are fine.   

First touch
-----------
Adjust the code to take into account the first touch policy
