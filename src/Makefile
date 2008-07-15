BIN         = haitus
CC          = gcc #llvm-gcc
LD          = gcc #llvm-ld -O5
DEBUGFLAGS  = -Wall -DNO_OUTPUT #-DDEBUG -g
OPTFLAGS    = -O3 -fomit-frame-pointer
CCFLAGS     = $(OPTFLAGS) $(DEBUGFLAGS)
OBJFLAGS    = #-emit-llvm
OBJFILES    = main.o   \
	      vm.o     \
              file.o   \
	      util.o

all: $(OBJFILES)
	$(LD) -o $(BIN) $(OBJFILES)

clean:
	rm -f *\.o *\~ $(BIN) $(BIN).dSYM

# source file building
.c.o:
	$(CC) $(CCFLAGS) $(OBJFLAGS) -c $<
