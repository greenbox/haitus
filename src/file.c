/*
    file.c - file loading routines
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
#include "file.h"


// must deallocate memory returned
program *load_program_from_file(int verbosity, char* file) {
  program *prog        = NULL;
  FILE    *fd          = NULL;
  char    *programcode = NULL;
  int      proglen     = 0;
  int      dataoffset  = 0;

  // we're declaring this static so it's saved and not mangled
  // when this function's stack frame returns and we go through several
  // other calls; if we don't declare this static then the opcodes will get
  // mangled by the time we get to vm_engine
  // luckily in the future we'll be dynamically allocating memory so this won't
  // be a problem, but I spent about 20 fucking minutes debugging this so I'm
  // commenting
  static   int      ex1prog[]      = { 11, 0, 0, 0 };

  fd = fopen(file,"r");
  if(fd == NULL)
    return NULL;

  prog = malloc(sizeof(program));
#ifdef DEBUG
  printf("load_program_from_file: allocated memory (%p) of size %d\n",prog,sizeof(program));
#endif

  // example to make sure this works
  prog->ip       = ex1prog;
  prog->dataseg  = NULL;
  prog->proglen  = 4;

#ifdef DEBUG
  if(prog->dataseg == NULL)
    printf("load_program_from_file: NULL data segment detected; no strings exist\n");
#endif

#ifdef DEBUG
  printf("load_program_from_file: closing file and returning allocated program memory (addr %p)\n",prog);
#endif
  fclose(fd);
  return prog;
}
