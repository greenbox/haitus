AC_INIT([haitus],[0.1],[mad.one@gmail.com])
AM_INIT_AUTOMAKE([ -Wall -Werror foreign ])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([ Makefile src/Makefile ])

AC_PROG_CXX

AC_CHECK_HEADER([getopt.h])
AC_CHECK_PROGS([llvm_config],[llvm-config],[:])
if test "$llvm_config" = :; then
   AC_MSG_ERROR([could not find llvm-config!])
fi

AC_SUBST([llvm_libs],[`$llvm_config --libs`])
AC_SUBST([llvm_cppflags],[`$llvm_config --cppflags`])
AC_SUBST([llvm_ldflags],[`$llvm_config --ldflags`])

AC_OUTPUT
