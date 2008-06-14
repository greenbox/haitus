/*
    main.c - driver that runs example vm programs
    Copyright (C) 2008  Austin Seipp

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
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
  time_t t1,t2;

  while((c = getopt_long(ac,av,"vVh", longopts, NULL)) != -1) {
    switch(c) {
    case 'v':
      verbosity++;
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

  /*
  if(1 > ac) {
    printf("Need a file.\n");
    exit(-1);
  } 
  printf("Loading file %s...\n",av[0]);
  */
  t1 = time(NULL);
#ifdef USE_SWITCH
  printf("Using switch interpreter...\n");
  run_program_switch("nil",verbosity,1);
  run_program_switch("nil",verbosity,2);
  run_program_switch("nil",verbosity,3);
  run_program_switch("nil",verbosity,4);
#else
  printf("Using dispatch interpreter...\n");
  run_program_dispatch("nil",verbosity,1);
  run_program_dispatch("nil",verbosity,2);
  run_program_dispatch("nil",verbosity,3);
  run_program_dispatch("nil",verbosity,4);
#endif
  t2 = time(NULL);
  printf("Time spent: %f\n",difftime(t2,t1));

  printf("Using direct threaded...\n");
  program_direct_threaded();

  return 0;
}
