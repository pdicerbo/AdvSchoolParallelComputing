Calculate Pi: effect of flase sharing
=====================================

Adjust Pi program
------------------
We provide you with a C source code to calculate Pi. 
Please adjust the omp parallel statement by chosing the right  
variables scope (private, shared). 

False sharing 
-------------

Compile the code with no optimization and debugging symbols

```
-g -O0
```

and try to show the effect of false sharing on the array sum[]
with  Vtune Memory collection. 

Add the affinity function built in the previous exercise, and
try to see if placing threads on a single socket or multiple socket
make some difference. Again check it with Vtune. 


Padded solution
---------------

Fix the false sharing by adding a dimension to array sum. Bear in 
mind that cache line is 64 bytes and we want consecutive elements 
of sum being in two different line of cache. 

Compile again the new code with 

```
-g -O0
```
 
and check if different placing makes difference in terms of performance

 
