EX2 : Vectorized Image Library
===============================

Follow the instruction in image.c and write a vectorized library to Image elaboration.
Verify the speedup obtained when you compile with AVX flag.

Env :

    module load autoload vtune python
    source /marconi_scratch/userinternal/epascol1/adv_py_school/bin/activate

Compilation :

To generate AVX library:

    make avxlib

To generate no-vec library:

    make noveclib

Launch line : 

    python3.5 image.py -i <image.jpg> -op <number o <operation>