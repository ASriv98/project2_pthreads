CC=g++
CFLAGS=-pthread -Iinclude -std=c++11
HEADER=include/parallel_prims.h

default: parallel_prims

dirs:
	/bin/mkdir -p objs/

clean:
	/bin/rm -rf objs *~ prims parallel_prims

prims: dirs  objs/prims.o
	$(CC) $(CFLAGS) -o $@ objs/prims.o

objs/prims.o: prims.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

parallel_prims: dirs objs/parallel_prims.o
	$(CC) $(CFLAGS) -o $@ objs/parallel_prims.o

objs/parallel_prims.o: parallel_prims.cpp $(HEADER)
	$(CC) $(CFLAGS) -c -o $@ $<
