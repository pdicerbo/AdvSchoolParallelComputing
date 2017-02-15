CC=icc
F90=ifort
F90FLAG=-qopenmp


all: 3d_update 3d_update_no_fs 3d_update_no_fs2 3d_update_no_fs_ft

3d_update: 3d_update.o affinity.o
	$(F90) $(F90FLAG) $^  -o $@	

3d_update_no_fs: 3d_update_no_fs.o affinity.o
	$(F90) $(F90FLAG) $^  -o $@	

3d_update_no_fs2: 3d_update_no_fs2.o affinity.o
	$(F90) $(F90FLAG) $^  -o $@	

3d_update_no_fs_ft: 3d_update_no_fs_ft.o affinity.o
	$(F90) $(F90FLAG) $^  -o $@	

3d_update.o: 3d_update.f90
	$(F90) $(F90FLAG) -assume nounderscore -c $^

3d_update_no_fs.o: 3d_update_no_fs.f90
	$(F90) $(F90FLAG) -assume nounderscore -c $^

3d_update_no_fs2.o: 3d_update_no_fs2.f90
	$(F90) $(F90FLAG) -assume nounderscore -c $^

3d_update_no_fs_ft.o: 3d_update_no_fs_ft.f90
	$(F90) $(F90FLAG) -assume nounderscore -c $^

affinity.o: affinity.c
	$(CC)  -c $^

clean:
	rm -f *.o *.x *~ *.dat *.mod

