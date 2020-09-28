CC = gcc-7
EXECS = sequential parallel

.PHONY: $(EXECS)
all: $(EXECS)

sequential:
	cd src;	$(CC) rcmSequential.c secondaryFunctions.c main.c -fopenmp -o  ../bin/$@; cd ..;

parallel:
	cd src;	$(CC) rcmOpenMP.c secondaryFunctions.c main.c  -fopenmp -o  ../bin/$@; cd ..;
