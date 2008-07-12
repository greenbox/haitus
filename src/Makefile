BIN         = haitus
CC          = gcc #llvm-gcc
LD          = gcc #llvm-ld -O5
DEBUGFLAGS  = -DNO_OUTPUT #-DDEBUG -g
OPTFLAGS    = -O0
CCFLAGS     = $(OPTFLAGS) $(DEBUGFLAGS)
OBJFLAGS    = #-emit-llvm
OBJFILES    = main.o   \
	      engine.o \
              file.o   \
	      util.o

all: $(OBJFILES)
	$(LD) -o $(BIN) $(OBJFILES)

clean:
	rm -f *\.o *\~ $(BIN) $(BIN).dSYM

# source file building
.c.o:
	$(CC) $(CCFLAGS) $(OBJFLAGS) -c $<
