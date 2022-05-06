#MAKEFILE FOR PROJECT 2 READER_WRITER_2
ODIR=obj
INCL =../includes


CC=clang
CFLAGS=-I$(INCL)



_DEPS = definitions.h file-op.h
DEPS := $(patsubst %,$(INCL)/%,$(_DEPS))

_OBJS = definitions.o file-op.o project_2.o
OBJS := $(patsubst %,$(ODIR)/%,$(_OBJS))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CLFAGS)

project_2: $(OBJS)
	$(CC) -o $@ $^ $(CLFAGS)

.PHONY clean

clean:
	rm -f $(ODIR)/*.o