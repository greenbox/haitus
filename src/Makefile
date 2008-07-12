BIN         = haitus
CC          = gcc
DEBUGFLAGS  = #-g -DDEBUG
OPTFLAGS    = -O3
CCFLAGS     = $(OPTFLAGS) $(DEBUGFLAGS)
OBJFLAGS    = 
OBJFILES    = main.o   \
	      engine.o \
              file.o 

all: $(OBJFILES)
	$(CC) -o $(BIN) $(OBJFILES)

clean:
	rm -f *\.o *\~ $(BIN) $(BIN).dSYM

# source file building
.c.o:
	$(CC) $(CCFLAGS) $(OBJFLAGS) -c $<
