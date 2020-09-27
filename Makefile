CC = gcc-7
EXECS = sequential

.PHONY: $(EXECS)
all: $(EXECS)
sequential:
	mkdir bin; 
	cd src;	$(CC) rcm.c secondaryFunctions.c main.c  -fopenmp -o  ../bin/$@; cd ..;
