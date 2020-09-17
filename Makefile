CC = gcc-7
EXECS = sequential

.PHONY: $(EXECS)
all: $(EXECS)
sequential:
	cd src;	$(CC) sequential.c secondaryFunctions.c main.c -o $@;