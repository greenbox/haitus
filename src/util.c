/*
    util.c - utility functions
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
#include <stdarg.h>
#include "haitus.h"

#ifndef NO_OUTPUT
// verbose printf to remove lots of if statements
int verboseprintf(char *fmt, ...) {
  va_list va;
  int x;

  if(option_verbose != 0) {
    va_start(va,fmt);
    if(option_verbose >= 3)
      x = vprintf(fmt,va);
    else
      x = 0;
    va_end(va);
    return x;
  } else
    return 0;
}
#endif /* !defined(NO_OUTPUT) */
