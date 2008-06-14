#include <stdio.h>
#include <stdlib.h>
#include "engine.h"

/* program bytecodes (# means number):
    0, x, n       -- set register #x to value n
    1, reg1, reg2 -- add val in #reg2 to val in #reg1 and store in #reg1
    2, x          -- show reg #x
    3             -- exit
 */

int program[] = { 0, 0, 10,
		  0, 1, 20,
		  1, 1, 0,
		  2, 1,
                  3 };


void run_file(char *file, int verbosity) { // we ignore the file for now
  void *func_table[] = { &&set, &&add, &&show, &&exit };
  int  regs[]        = { 0, 0, 0, 0, 0, 0, 0 };
  int  *ip           = &program;

  *func_table[ip[0]];


set:
  if (verbosity > 0)
    printf("[op 0] setting reg %d to val %d\n",ip[1],ip[2]);

  regs[ip[1]] = ip[2];
  ip += 3; 

  if (verbosity > 0)
    printf("[op 0] next op is %d\n",ip[0]);

  goto *func_table[ip[0]];

add:
  if (verbosity > 0)
    printf("[op 1] adding reg%d(val=%d) to reg%d(val=%d), reg%d=%d\n",
	   ip[1],regs[ip[1]],ip[2],regs[ip[2]],ip[1],
	   regs[ip[1]] + regs[ip[2]]);

  regs[ip[1]] = regs[ip[1]] + regs[ip[2]];
  ip += 3;

  if (verbosity > 0)
    printf("[op 1] next op is %d\n",ip[0]);

  goto *func_table[ip[0]];

show:
  printf("[op 2] reg %d ==> %d\n",ip[1],regs[ip[1]]);
  ip += 2; 

  if (verbosity > 0)
    printf("[op 2] next op is %d\n",ip[0]);

  goto *func_table[ip[0]];

exit:
  return;
}
