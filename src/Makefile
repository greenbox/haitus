BIN         = haitus
CC          = gcc #llvm-gcc
LD          = gcc #llvm-ld -O5
DEBUGFLAGS  = -Wall -DNO_OUTPUT #-DDEBUG -g
OPTFLAGS    = -O3 -fomit-frame-pointer
CCFLAGS     = $(OPTFLAGS) $(DEBUGFLAGS)
OBJFLAGS    = #-emit-llvm
CFILES      = util.c file.c vm.c haitus.c

all: $(OBJFILES)
	$(CC) -o $(BIN) $(CFILES)

clean:
	rm -f *\.o *\~ $(BIN) $(BIN).dSYM
