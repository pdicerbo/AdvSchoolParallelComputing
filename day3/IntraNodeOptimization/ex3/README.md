OpenMP 4.5 affinity features
============================


C Fuction 
---------

Write a C function that prints to standard output

  * the number of OpenMP places
  * the size of each OpenMP places
  * the processor ids available in each place
 

make use of OpenMP 4.5 new functions 

```
    omp_get_num_places
    omp_get_place_num_procs
    omp_get_place_proc_ids
```

Check the prototypes using the standard docs

  http://www.openmp.org/wp-content/uploads/openmp-4.5.pdf
  http://www.openmp.org/wp-content/uploads/OpenMP-4.5-1115-CPP-web.pdf


Test code
---------

Write a test code that exemplifies the code just written. 
Try different combination of places and bind with the variables
through the environment variables 

  * OMP_PROC_BIND
  * OMP_PLACES 

To receive confirmation of all the settings we suggest to use

```
  export OMP_DISPLAY_ENV=TRUE
```


