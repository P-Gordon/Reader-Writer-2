#MAKEFILE FOR PROJECT 2 READER_WRITER_2
ODIR=obj
INCL=-I ../include
LIBS=-lrt -lpthread
CC=clang
CFLAGS=-Wall -std=c18
DEPS = defs.h file-op.h thread-work.h
OBJS = defs.o file-op.o thread-work.o project_2.o

project_2: $(OBJS)
	$(CC) $(CFLAGS) $(INCL) $^ -o $@ $(LIBS)

%.o : %.c 
	$(CC) $(CFLAGS) $(INCL) -c $<


.PHONY: clean

clean:
	rm -f *.o