CC=g++
CFLAGS=-pthread -Iinclude
HEADER=include/password_crack.h

default: crack

dirs:
	/bin/mkdir -p objs/

clean:
	/bin/rm -rf objs *~ crack

crack: dirs  objs/password_crack.o
	$(CC) $(CFLAGS) -o $@ objs/password_crack.o

objs/password_crack.o: password_crack.cpp $(HEADER)
	$(CC) $(CFLAGS) -c -o $@ $<