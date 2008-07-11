BIN         = haitus
CC          = gcc
DEBUGFLAGS  = #-g -DDEBUG
OPTFLAGS    = -O3
CCFLAGS     = $(OPTFLAGS) $(DEBUGFLAGS)
OBJFLAGS    = 
OBJFILES    = main.o   \
	      engine.o

all: $(OBJFILES)
	$(CC) -o $(BIN) $(OBJFILES)

clean:
	rm -f *\.o *\~ $(BIN) $(BIN).dSYM

# source file building
main.o:
	$(CC) $(CCFLAGS) $(OBJFLAGS) -c main.c

engine.o:
	$(CC) $(CCFLAGS) $(OBJFLAGS) -c engine.c