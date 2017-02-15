avxlib : image.c
	icc -qopenmp -g  -xavx -std=c11 -qopenmp -qopt-report5 -c -O2 -fpic image.c -o image.o
	icc -shared image.o -qopenmp -o libimage.so

noveclib : image.c
	icc -qopenmp -g  -no-vec -std=c11 -qopenmp -qopt-report5 -c -O2 -fpic image.c -o image.o
	icc -shared image.o -qopenmp -o libimage.so


clean :
	rm *.so *.o *.optrpt

