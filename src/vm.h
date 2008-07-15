#ifndef _VM_H
#define _VM_H

int run_program_static(int program);
int run_program_from_file(char *file);

int vm_engine(int *ip, void **dataseg);

#endif /* !defined(_VM_H) */
