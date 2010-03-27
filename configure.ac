#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.61])
AC_INIT([gsimplecal], [0.2-hg], [http://code.google.com/p/gsimplecal/issues])

AM_INIT_AUTOMAKE

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/gsimplecal.cpp])

# Checks for programs.
AC_LANG([C++])
AC_PROG_CXXCPP
AC_PROG_CXX
AC_PROG_INSTALL
PKG_PROG_PKG_CONFIG

# Checks for libraries.
#PKG_CHECK_MODULES([UNIQUE], [unique])
#AC_SUBST(LIBUNIQUE_CFLAGS)
#AC_SUBST(LIBUNIQUE_LIBS)

# Checks for header files.
AC_CHECK_HEADERS([iostream])
AC_CHECK_HEADERS([string])
AC_CHECK_HEADERS([sstream])
AC_CHECK_HEADERS([fstream])
AC_CHECK_HEADERS([vector])
AC_CHECK_HEADERS([cstdlib])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([gtk/gtk.h])
AC_CHECK_HEADERS([unique/unique.h])
AC_CHECK_HEADERS([glib/gstdio.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([atexit gettimeofday setenv])

#
AM_CFLAGS="-Wall"
AM_LDFLAGS="-Wl,--as-needed"
AC_SUBST(AM_CFLAGS)
AC_SUBST(AM_LDFLAGS)

DISTCLEANFILES="Makefile.in"
AC_SUBST(DISTCLEANFILES)

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([doc/Makefile])
AC_CONFIG_FILES([src/Makefile])

AC_OUTPUT
