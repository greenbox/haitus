#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <llvm-c/Core.h>
#include "config.h"
#include "engine.h"

static struct option longopts[] = {
  { "verbose", no_argument, NULL, 'v'},
  { "version", no_argument, NULL, 'V'},
  { "help",    no_argument, NULL, 'h'},
  { NULL, 0, NULL, 0 }
};

void version() {
  printf(PACKAGE_STRING ": simple LLVM virtual machine.\n");
  exit(0);
}

void usage() {
  printf("usage: executable [options] [file]...\n");
  printf("'file' is a bytecode file to load.\n");
  printf("available options:\n");
  printf(" --verbose\t-v\t\tincrease verbosity\n");
  printf(" --version\t-V\t\tshow version information\n");
  printf(" --help\t\t-h\t\tshow this information\n");
  exit(0);
}

int main(int ac, char **av) {
  int c;
  int verbosity = 0;

  while((c = getopt_long(ac,av,"vVh", longopts, NULL)) != -1) {
    switch(c) {
    case 'v':
      verbosity = 1;
      break;
    case 'V':
      version();
    case 'h':
    case '?':
    default:
      usage();
    }
  }

  ac -= optind;
  av += optind;

  if(1 > ac) {
    printf("Need a file.\n");
    exit(-1);
  } 
  printf("Loading file %s...\n",av[0]);
  run_file(av[0],verbosity);

  return 0;
}
