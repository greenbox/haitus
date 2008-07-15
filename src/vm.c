/*
    vm.c - actual virtual machine
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
#include <stdarg.h>
#include <math.h>
#include "file.h"

/* we use a RISC-like approach; all opcodes are of the same length
   and we use relative offsets for jumps. all opcodes are 32-bits
   in length (4 seperate bytes)
   program bytecodes (# means number):
    0,  x,  n,  0  -- set register #x to value n
    0,  r1, r2, 1  -- set register #r1 to == register #r2
    1,  r1, r2, r3 -- add val in r2 to r3 and store in r1
    2,  r1, r2, r3 -- sub val in r2 by r3 and store in r1
    3,  r1, r2, r3 -- multiply val in r2 by r3 and store in r1
    4,  r1, r2, r3 -- divide val in r2 by r3 and store in r1
    5,  r1, r2, r3 -- mod val in r2 by r3 and store in r1
    6,  x,  0,  0  -- show reg #x
    7,  n,  0,  0  -- jump #n opcodes back/forwards (depends if it's positive/negative)
    8,  r,  v,  0  -- do-next-if: execute next instruction if val in reg 'r' == v
    9,  r,  v,  0  -- skip-next-if: skip next instruction if val in reg 'r'  == v
    10, r,  0,  0  -- print string at data index 'r'
    11, n,  0,  0  -- exit with value 'n'
  */

void* staticdata[] = { "fizz", "buzz", "hello world!", "goodbye world!" };


// addition
int program1[] = { 0,  0, 10, 0,   // reg 0 = 10
                   0,  1, 20, 0,   // reg 1 = 20
                   1,  1, 1,  0,   // reg 1 = reg1 + reg0
                   6,  1, 0,  0,   // show reg 1
                   11, 0, 0,  0 }; // exit

// loop
int program2[] = { 0, 0,    10,   0,   // reg 0 = 10
                   0, 1,    1,    0,   // reg 1 = 1
                   6, 0,    0,    0,   // show reg 0
                   2, 0,    0,    1,   // reg 0 = reg 0 - reg 1
                   9, 0,    0,    0,   // skip next if reg 0 == 0
                   7, (-3), 0,    0,   // jump back 3 instrs
                   11, 0,    0,    0 }; // exit

// hello world
int program3[] = { 10, 2, 0, 0,   // print "hello world!"
                   10, 3, 0, 0,   // print "goodbye world!"
                   11, 0, 0, 0 }; // exit

// fizzbuzz
int program4[] = { 0,    0, 15,  0,   // reg 0 = 15 
                   0,    1, 1,   0,   // reg 1 = 1 
                   0,    2, 3,   0,   // reg 2 = 3
                   0,    3, 5,   0,   // reg 3 = 5
                   5,    4, 0,   2,   // reg 4 = reg0 % reg2
                   5,    5, 0,   3,   // reg 5 = reg0 % reg3
                   6,    0, 0,   0,   // show reg 0
                   8,    4, 0,   0,   // do next if r4 == 0
                   10,   0, 0,   0,   // print 'fizz'
                   8,    5, 0,   0,   // do next if r5 == 0
                   10,   1, 0,   0,   // print 'buzz'
                   2,    0, 0,   1,   // reg 0 = reg 0 - reg 1
                   9,    0, 0,   0,   // skip next if r0 == 0
                   7, (-10), 0,  0,   // jump back 10 instructions
                   11,   0, 0,   0 }; // exit


// runs the programs above with their static data section
int run_program_static(int verbosity, int program) {
  int *ip;

  if (program == 1)
    ip = program1;
  else if (program == 2)
    ip = program2;
  else if (program == 3)
    ip = program3;
  else if(program == 4)
    ip = program4;
  else {
    printf("Incorrect program\n");
    exit(-1);
  }

  return vm_engine(verbosity, ip, staticdata);
}


// executes a program from a file
int run_program_from_file(int verbosity, char* file) {
  program *prog = (program*)load_program_from_file(verbosity,file);
  int ret       = 0;

  if(prog == NULL) {
    printf("Error loading from file.\n"); ret = -1;
  } else {
#ifdef DEBUG
    printf("run_program_from_file: ip is at %p\n",prog->ip);
    printf("run_program_from_file: first detected opcode is %d\n",(prog->ip)[0]);
#endif
    ret = vm_engine(verbosity,prog->ip,prog->dataseg);
    free(prog);
  }

  return ret;
}


// main vm engine, runs a program with a specified data
// segment
int vm_engine(int verbosity, int *ip, void **dataseg) { 
  void *func_table[] = { &&set,      // byte 0
			 &&add,      // byte 1
			 &&sub,      // byte 2
			 &&mul,      // byte 3
			 &&div,      // byte 4
			 &&mod,      // byte 5
			 &&show,     // byte 6
			 &&jump,     // byte 7
			 &&nextif,   // byte 8
			 &&skipif,   // byte 9
			 &&printstr, // byte 10
			 &&exit };   // byte 11
  int  regs[]        = { 0, 0, 0, 0, 0, 0, 0, 0, // 32 registers
                         0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 0, 0, 0, 0 };


#ifdef DEBUG
  printf("First opcode to be executed: %d\n",ip[0]);
#endif

  goto *func_table[ip[0]];

 set:
  if (ip[3] == 0) {
    verboseprintf(verbosity,"[op 0] setting reg %d to val %d\n",ip[1],ip[2]);
    regs[ip[1]] = ip[2];
  } else if (ip[3] == 1) {
    verboseprintf(verbosity,"[op 0] setting reg %d to reg%d(%d)\n",ip[1],ip[2],regs[ip[2]]);
    regs[ip[1]] = regs[ip[2]];
  }
  
  ip += 4; 
  verboseprintf(verbosity,"[op 0] next op is %d\n",ip[0]);

  goto *func_table[ip[0]];

 add:
  verboseprintf(verbosity,"[op 1] setting reg%d = reg%d + reg%d (%d)\n",
		ip[1],ip[2],ip[3], regs[ip[2]] + regs[ip[3]]);
  
  regs[ip[1]] = regs[ip[2]] + regs[ip[3]];
  ip += 4;

  verboseprintf(verbosity,"[op 1] next op is %d\n",ip[0]);
  
  goto *func_table[ip[0]];
  
 sub:
  verboseprintf(verbosity,"[op 2] setting reg%d = reg%d - reg%d (%d)\n",
		ip[1],ip[2],ip[3], regs[ip[2]] - regs[ip[3]]);
  
  regs[ip[1]] = regs[ip[2]] - regs[ip[3]];
  ip += 4;
  
  verboseprintf(verbosity,"[op 2] next op is %d\n",ip[0]);
  
  goto *func_table[ip[0]];
  
 mul:
  verboseprintf(verbosity,"[op 3] setting reg%d = reg%d * reg%d (%d)\n",
		ip[1],ip[2],ip[3], regs[ip[2]] * regs[ip[3]]);
  
  regs[ip[1]] = regs[ip[2]] * regs[ip[3]];
  ip += 4;
  
  verboseprintf(verbosity,"[op 3] next op is %d\n",ip[0]);
  goto *func_table[ip[0]];
  
 div:
  verboseprintf(verbosity,"[op 4] setting reg%d = reg%d / reg%d (%d)\n",
		ip[1],ip[2],ip[3], regs[ip[2]] / regs[ip[3]]);
  
  regs[ip[1]] = regs[ip[2]] / regs[ip[3]];
  ip += 4;
  
  verboseprintf(verbosity,"[op 4] next op is %d\n",ip[0]);
  goto *func_table[ip[0]];
  
 mod:
  verboseprintf(verbosity,"[op 5] setting reg%d = reg%d + reg%d (%d)\n",
		ip[1],ip[2],ip[3], regs[ip[2]] + regs[ip[3]]);
  
  regs[ip[1]] = regs[ip[2]] % regs[ip[3]];
  ip += 4;
  
  verboseprintf(verbosity,"[op 5] next op is %d\n",ip[0]);
  goto *func_table[ip[0]];
  
 show:
#ifndef NO_OUTPUT
  printf("[op 6] reg %d ==> %d\n",ip[1],regs[ip[1]]);
#endif
  ip += 4; 

  verboseprintf(verbosity,"[op 6] next op is %d\n",ip[0]);

  goto *func_table[ip[0]];

 jump:
  verboseprintf(verbosity,"[op 7] jumping %s %d opcodes\n",
		ip[1] > 0 ? "forward" : "back",abs(ip[1]));

  ip += (ip[1]*4);

  verboseprintf(verbosity,"[op 7] next op is %d\n",ip[0]);

  goto *func_table[ip[0]];

 nextif:
  verboseprintf(verbosity,"[op 8] executing next instruction (op %d) if reg%d == %d\n",
		ip[4],ip[1], ip[2]);

  if(regs[ip[1]] == ip[2])
    ip += 4;
  else
    ip += 8;

  verboseprintf(verbosity,"[op 8] next op is %d\n",ip[0]);
  
  goto *func_table[ip[0]];

 skipif:
  verboseprintf(verbosity,"[op 9] skipping next instruction (op %d) if reg%d == %d\n",
		ip[4],ip[1], ip[2]);

  if(regs[ip[1]] == ip[2])
    ip += 8;
  else
    ip += 4;

  verboseprintf(verbosity,"[op 9] next op is %d\n",ip[0]);

  goto *func_table[ip[0]];

 printstr:
  verboseprintf(verbosity,"[op 10] printing string, data index %d\n",ip[1]);

#ifndef NO_OUTPUT  
  printf("%s\n",dataseg[ip[1]]);
#endif
  ip += 4;
  
  verboseprintf(verbosity,"[op 10] next op is %d\n",ip[0]);
  
  goto *func_table[ip[0]];

 exit:
  verboseprintf(verbosity,"[op 11] exiting with value %d\n",ip[1]);
  return ip[1];
}
