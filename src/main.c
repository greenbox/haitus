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

static struct option longopts[] = {
  { "verbose",    required_argument, NULL, 'v'},
  { "version",    no_argument,       NULL, 'V'},
  { "help",       no_argument,       NULL, 'h'},
  { "program",    required_argument, NULL, 'p'},
  { "iterate",    required_argument, NULL, 'i'},
  { NULL, 0, NULL, 0 }
};

void version() {
  printf("haitus v0.1: simple virtual machine\n");
  exit(0);
}

void usage() {
  printf("usage: executable [options] [file]...\n");
  printf("'file' is a bytecode file to load.\n");
  printf("available options:\n");
  printf(" --verbose=n\t-v n\t\tset verbosity to level n\n");
  printf(" --version\t-V\t\tshow version information\n");
  printf(" --help\t\t-h\t\tshow this information\n");
  printf(" --program=n\t-p n\t\texecute program 'n'\n");
  printf(" --iterate=n\t-i n\t\trun program 'n' times, must be used with --program\n");
  exit(0);
}

int main(int ac, char **av) {
  int c;
  int verbosity = 1;
  int ret  = 0;
  int i = 0, iterations = 1;
  int prog = -1;

  while((c = getopt_long(ac,av,"v:Vhp:i:", longopts, NULL)) != -1) {
    switch(c) {
    case 'v':
      verbosity = atoi(optarg);
      break;
    case 'V':
      version();
      break;
    case 'p':
      prog = atoi(optarg);
      break;
    case 'i':
      iterations = atoi(optarg);
      break;
    case 'h':
    case '?':
    default:
      usage();
    }
  }

  ac -= optind;
  av += optind;

  if(av[0]) {
    printf("Executing file %s\n",av[0]);
    ret = run_program_from_file(verbosity,av[0]);
  } else {
    if(prog == -1) {
      printf("Executing program1:\n"); ret &= run_program_static(verbosity,1);
      printf("Executing program2:\n"); ret &= run_program_static(verbosity,2);
      printf("Executing program3:\n"); ret &= run_program_static(verbosity,3);
      printf("Executing program4:\n"); ret &= run_program_static(verbosity,4);
    } else {
      printf("Running program %d %d times.\n",prog,iterations);
      for(i = 0; i < iterations; i++)
	ret & run_program_static(verbosity,prog);
    }
  }
  return ret;
}
