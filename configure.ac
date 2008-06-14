AC_INIT([haitus],[0.1],[mad.one@gmail.com])
AM_INIT_AUTOMAKE([ -Wall -Werror foreign ])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([ Makefile src/Makefile ])

AC_PROG_CC
AC_PROG_CXX

AC_CHECK_PROGS([llvm_config],[llvm-config],[:])
if test "$llvm_config" = :; then
   AC_MSG_ERROR([could not find llvm-config!])
fi

AC_SUBST([llvm_libs],[`$llvm_config --libs`])
AC_SUBST([llvm_include], [`$llvm_config --includedir`])

AC_OUTPUT
