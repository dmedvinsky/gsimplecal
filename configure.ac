#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.65])
AC_INIT([gsimplecal], [0.2-hg], [http://code.google.com/p/gsimplecal/issues], [gsimplecal], [http://code.google.com/p/gsimplecal/])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_CONFIG_SRCDIR([src/gsimplecal.cpp])

# Checks for programs.
AC_LANG([C++])
AC_PROG_CXX
AC_PROG_INSTALL
PKG_PROG_PKG_CONFIG

# Checks for libraries.
PKG_CHECK_MODULES([LIBUNIQUE], [unique-1.0])
AC_SUBST(LIBUNIQUE_CFLAGS)
AC_SUBST(LIBUNIQUE_LIBS)

PKG_CHECK_MODULES([GTK2], [gtk+-2.0])
AC_SUBST(GTK2_CFLAGS)
AC_SUBST(GTK2_LIBS)

# Checks for header files.
AC_CHECK_HEADERS([iostream string sstream fstream vector cstdlib stdlib.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([atexit gettimeofday setenv])

#
AM_CPPFLAGS="-Wall"
AM_LDFLAGS="-Wl,--as-needed"
AC_SUBST(AM_CFLAGS)
AC_SUBST(AM_LDFLAGS)

AC_CONFIG_FILES([Makefile
                 doc/Makefile
                 src/Makefile])

AC_OUTPUT
