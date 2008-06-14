/*
    engine.c - actual virtual machine
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
#include <math.h>
#include "engine.h"

/* we use a RISC-like approach; all opcodes are of the same length
   and we use relative offsets for jumps. all opcodes are 32-bits
   in length (4 seperate bytes)
   program bytecodes (# means number):
    0,  x,  n,  0 -- set register #x to value n
    1,  r1, r2, 0 -- add val in #r2 to val in #r1 and store in #r1
    2,  r1, r2, 0 -- sub val in #r2 from val in #r1 and store in #r1
    3,  r1, r2, 0 -- multiply val in #r1 by #r2 and store in #r1
    4,  r1, r2, 0 -- divide val in #r1 by #r2 and store in #r1
    5,  r1, r2, 0 -- mod val in #r1 by #r2 and store in #r1
    6,  x,  0,  0 -- show reg #x
    7,  n,  0,  0 -- jump #n opcodes back/forwards (depends on value)
    8,  r,  v,  0 -- do-next-if: execute next instruction if val in reg 'r' == v
    9,  r,  v,  0 -- skip-next-if: skip next instruction if val in reg 'r'  == v
    10, 0,  0,  0 -- exit
 */

// addition
int program1[] = { 0,  0, 10, 0,   // reg 0 = 10
	      	   0,  1, 20, 0,   // reg 1 = 20
 		   1,  1, 0,  0,   // reg 1 = reg1 + reg0
		   6,  1, 0,  0,   // show reg 1
                   10, 0, 0,  0 }; // exit

// loop
int program2[] = { 0, 0,    10,   0,   // reg 0 = 10
		   0, 1,    1,    0,   // reg 1 = 1
                   6, 0,    0,    0,   // show reg 0
                   2, 0,    1,    0,   // reg 0 = reg 0 - reg 1
                   9, 0,    0,    0,   // skip next if reg 0 == 0
                   7, (-3), 0,    0,   // jump back 3 instrs
                   10, 0,    0,    0 }; // exit

// fizzbuzz
int program3[] = { };

void run_program_switch(char *file, int verbosity, int program) {
  int regs[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  int *ip;

  if (program == 1)
    ip = &program1;
  else
    ip = &program2;

  for(;;) {
    switch(ip[0]) {
    case 0:
      if (verbosity > 0)
	printf("[op 0] setting reg %d to val %d\n",ip[1],ip[2]);

      regs[ip[1]] = ip[2];
      ip += 4; 

      if (verbosity > 0)
	printf("[op 0] next op is %d\n",ip[0]);

      break;

    case 1:
      if (verbosity > 0)
	printf("[op 1] adding reg%d(val=%d) to reg%d(val=%d), reg%d=%d\n",
	       ip[1],regs[ip[1]],ip[2],regs[ip[2]],ip[1],
	       regs[ip[1]] + regs[ip[2]]);
      
      regs[ip[1]] = regs[ip[1]] + regs[ip[2]];
      ip += 4;
      
      if (verbosity > 0)
	printf("[op 1] next op is %d\n",ip[0]);
      
      break;

    case 2:
      if (verbosity > 0)
	printf("[op 2] adding reg%d(val=%d) to reg%d(val=%d), reg%d=%d\n",
	       ip[1],regs[ip[1]],ip[2],regs[ip[2]],ip[1],
	       regs[ip[1]] + regs[ip[2]]);
      
      regs[ip[1]] = regs[ip[1]] - regs[ip[2]];
      ip += 4;
      
      if (verbosity > 0)
	printf("[op 2] next op is %d\n",ip[0]);
      break;
      
    case 3:
      if (verbosity > 0)
	printf("[op 3] multiplying reg%d(val=%d) by reg%d(val=%d), reg%d=%d\n",
	       ip[1],regs[ip[1]],ip[2],regs[ip[2]],ip[1],
	       regs[ip[1]] * regs[ip[2]]);
      
      regs[ip[1]] = regs[ip[1]] * regs[ip[2]];
      ip += 4;
      
      if (verbosity > 0)
	printf("[op 3] next op is %d\n",ip[0]);
      break;
      
    case 4:
      if (verbosity > 0)
	printf("[op 4] dividing reg%d(val=%d) by reg%d(val=%d), reg%d=%d\n",
	       ip[1],regs[ip[1]],ip[2],regs[ip[2]],ip[1],
	       regs[ip[1]] / regs[ip[2]]);
      
      regs[ip[1]] = regs[ip[1]] / regs[ip[2]];
      ip += 4;
      
      if (verbosity > 0)
	printf("[op 4] next op is %d\n",ip[0]);
      break;
      
    case 5:
      if (verbosity > 0)
	printf("[op 5] modulus reg%d(val=%d) by reg%d(val=%d), reg%d=%d\n",
	       ip[1],regs[ip[1]],ip[2],regs[ip[2]],ip[1],
	       regs[ip[1]] % regs[ip[2]]);
      
      regs[ip[1]] = regs[ip[1]] % regs[ip[2]];
      ip += 4;
      
      if (verbosity > 0)
	printf("[op 5] next op is %d\n",ip[0]);
      break;
      
    case 6:
      
      printf("[op 2] reg %d ==> %d\n",ip[1],regs[ip[1]]);
      ip += 4; 
      
      if (verbosity > 0)
	printf("[op 2] next op is %d\n",ip[0]);
      break;

    case 7:
      if (verbosity > 0)
	printf("[op 3] jumping %s %d opcodes\n",
	       ip[1] > 0 ? "forward" : "back",abs(ip[1]));
      
      ip += (ip[1]*4);
      
      if (verbosity > 0)
	printf("[op 3] next op is %d\n",ip[0]);
      break;

    case 8:
      if (verbosity > 0)
	printf("[op 4] executing next instruction (op %d) if reg%d == %d\n",
	       ip[4],regs[ip[1]], ip[2]);
      
      if(regs[ip[1]] == ip[2])
	ip += 4;
      else
	ip += 8;
      
      if (verbosity > 0)
	printf("[op 4] next op is %d\n",ip[0]);
      break;

    case 9:
      if (verbosity > 0)
	printf("[op 5] skipping next instruction (op %d) if reg%d == %d\n",
	       ip[4],regs[ip[1]], ip[2]);
      
      if(regs[ip[1]] == ip[2])
	ip += 8;
      else
	ip += 4;
      
      if (verbosity > 0)
	printf("[op 5] next op is %d\n",ip[0]);
      break;
    case 10:
      if (verbosity > 0)
	printf("[op 6] exiting\n");
      goto end;
    }
  }

end:
  return;
}

void run_program_dispatch(char *file, int verbosity, int program) { // we ignore the file for now
  void *func_table[] = { &&set,    // byte 0
			 &&add,    // byte 1
			 &&sub,    // byte 2
			 &&mul,    // byte 3
			 &&div,    // byte 4
			 &&mod,    // byte 5
			 &&show,   // byte 6
			 &&jump,   // byte 7
			 &&nextif, // byte 8
			 &&skipif, // byte 9
			 &&exit }; // byte 10
  int  regs[]        = { 0, 0, 0, 0, 0, 0, 0, 0 };
  int  *ip;

  if(program == 1) {
    ip = &program1;
    printf("Executing program1:\n");
    goto *func_table[ip[0]];
  } else if (program == 2) {
    ip = &program2;
    printf("Executing program2:\n");
    goto *func_table[ip[0]];
  }

set:
  if (verbosity > 0)
    printf("[op 0] setting reg %d to val %d\n",ip[1],ip[2]);

  regs[ip[1]] = ip[2];
  ip += 4; 

  if (verbosity > 0)
    printf("[op 0] next op is %d\n",ip[0]);

  goto *func_table[ip[0]];

add:
  if (verbosity > 0)
    printf("[op 1] adding reg%d(val=%d) to reg%d(val=%d), reg%d=%d\n",
	   ip[1],regs[ip[1]],ip[2],regs[ip[2]],ip[1],
	   regs[ip[1]] + regs[ip[2]]);

  regs[ip[1]] = regs[ip[1]] + regs[ip[2]];
  ip += 4;

  if (verbosity > 0)
    printf("[op 1] next op is %d\n",ip[0]);

  goto *func_table[ip[0]];

sub:
  if (verbosity > 0)
    printf("[op 2] adding reg%d(val=%d) to reg%d(val=%d), reg%d=%d\n",
	   ip[1],regs[ip[1]],ip[2],regs[ip[2]],ip[1],
	   regs[ip[1]] + regs[ip[2]]);

  regs[ip[1]] = regs[ip[1]] - regs[ip[2]];
  ip += 4;

  if (verbosity > 0)
    printf("[op 2] next op is %d\n",ip[0]);

  goto *func_table[ip[0]];

mul:
  if (verbosity > 0)
    printf("[op 3] multiplying reg%d(val=%d) by reg%d(val=%d), reg%d=%d\n",
	   ip[1],regs[ip[1]],ip[2],regs[ip[2]],ip[1],
	   regs[ip[1]] * regs[ip[2]]);

  regs[ip[1]] = regs[ip[1]] * regs[ip[2]];
  ip += 4;

  if (verbosity > 0)
    printf("[op 3] next op is %d\n",ip[0]);

  goto *func_table[ip[0]];

div:
  if (verbosity > 0)
    printf("[op 4] dividing reg%d(val=%d) by reg%d(val=%d), reg%d=%d\n",
	   ip[1],regs[ip[1]],ip[2],regs[ip[2]],ip[1],
	   regs[ip[1]] / regs[ip[2]]);

  regs[ip[1]] = regs[ip[1]] / regs[ip[2]];
  ip += 4;

  if (verbosity > 0)
    printf("[op 4] next op is %d\n",ip[0]);

  goto *func_table[ip[0]];

mod:
  if (verbosity > 0)
    printf("[op 5] modulus reg%d(val=%d) by reg%d(val=%d), reg%d=%d\n",
	   ip[1],regs[ip[1]],ip[2],regs[ip[2]],ip[1],
	   regs[ip[1]] % regs[ip[2]]);

  regs[ip[1]] = regs[ip[1]] % regs[ip[2]];
  ip += 4;

  if (verbosity > 0)
    printf("[op 5] next op is %d\n",ip[0]);

  goto *func_table[ip[0]];

show:
  printf("[op 6] reg %d ==> %d\n",ip[1],regs[ip[1]]);
  ip += 4; 

  if (verbosity > 0)
    printf("[op 6] next op is %d\n",ip[0]);

  goto *func_table[ip[0]];

jump:
  if (verbosity > 0)
    printf("[op 7] jumping %s %d opcodes\n",
	   ip[1] > 0 ? "forward" : "back",abs(ip[1]));

  ip += (ip[1]*4);

  if (verbosity > 0)
    printf("[op 7] next op is %d\n",ip[0]);

  goto *func_table[ip[0]];

nextif:
  if (verbosity > 0)
    printf("[op 8] executing next instruction (op %d) if reg%d == %d\n",
	   ip[4],regs[ip[1]], ip[2]);

  if(regs[ip[1]] == ip[2])
    ip += 4;
  else
    ip += 8;

  if (verbosity > 0)
    printf("[op 8] next op is %d\n",ip[0]);

  goto *func_table[ip[0]];

skipif:
  if (verbosity > 0)
    printf("[op 9] skipping next instruction (op %d) if reg%d == %d\n",
	   ip[4],regs[ip[1]], ip[2]);

  if(regs[ip[1]] == ip[2])
    ip += 8;
  else
    ip += 4;

  if (verbosity > 0)
    printf("[op 9] next op is %d\n",ip[0]);

  goto *func_table[ip[0]];

exit:
  if (verbosity > 0)
    printf("[op 10] exiting\n");

  return;
}
