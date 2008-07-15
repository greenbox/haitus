/*
    vm.h - prototypes for vm functions
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
#ifndef _VM_H
#define _VM_H

typedef struct {
  int   *ip;
  void **dataseg;
  int    proglen;
} program;

int run_program_static(int program);
int run_program_from_file(char *file);

int vm_engine(int *ip, void **dataseg);

#endif /* !defined(_VM_H) */
