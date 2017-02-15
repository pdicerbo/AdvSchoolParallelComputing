
CIC = icc
CIFLAG = -xAVX2 -g -qopenmp -qopt-report4 -std=c99 -O2
  intel :
		$(CIC) $(CIFLAG) ./datatype_d.c -o dbl.xx

  clean:
		rm *.S *.optrpt *.xx

  all : intel
