CC=icc
F90=ifort
F90FLAG=-qopenmp


all: 3d_update 

3d_update: 3d_update.o affinity.o
	$(F90) $(F90FLAG) $^  -o $@	

3d_update.o: 3d_update.f90
	$(F90) $(F90FLAG) -assume nounderscore -c $^

affinity.o: affinity.c
	$(CC)  -c $^

clean:
	rm -f *.o *.x *~ *.dat *.mod

