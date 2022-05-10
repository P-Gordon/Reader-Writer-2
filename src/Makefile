#MAKEFILE FOR PROJECT 2 READER_WRITER_2
ODIR=obj
INCL=-I ../include
LIBS=-lrt -lpthread
CC=clang
CFLAGS=-std=c18 -Wall
DEPS = defs.h file_op.h thread_work.h
OBJS = defs.o file_op.o thread_work.o project_2.o

project_2: $(OBJS)
	$(CC) $(CFLAGS) $(INCL) $^ -o $@ $(LIBS)

%.o : %.c 
	$(CC) $(CFLAGS) $(INCL) -c $<


.PHONY: clean

clean:
	rm -f *.o