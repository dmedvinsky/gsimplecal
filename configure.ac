AC_PREREQ([2.65])
AC_INIT([gsimplecal],
        [1.6],
        [https://github.com/dmedvinsky/gsimplecal/issues],
        [gsimplecal],
        [https://github.com/dmedvinsky/gsimplecal])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_CONFIG_SRCDIR([src/gsimplecal.cpp])
AC_CONFIG_HEADERS([src/config.h])

# Support silent build rules.
# Disable by either passing `--disable-silent-rules` to `configure`
# or passing V=1 to `make`.
AM_SILENT_RULES([yes])


# Checks for programs.
AC_LANG([C++])
AC_PROG_CXX
AC_PROG_INSTALL
PKG_PROG_PKG_CONFIG


# Checks for libraries.

# GTK
AC_ARG_ENABLE(gtk2,
    AS_HELP_STRING([--enable-gtk2],
                   [Build against GTK2. By default gsimplecal uses GTK3, but if
                    you set this flag, it will use GTK2 instead.]),
    [enable_gtk2=$enableval],
    [enable_gtk2=no]
)
AM_CONDITIONAL([ENABLE_GTK2], [test x"$enable_gtk2" = x"yes"])
AM_COND_IF([ENABLE_GTK2],
           [GTK_VERSION=gtk+-2.0],
           [GTK_VERSION=gtk+-3.0])

AC_DEFINE([HAVE_GTK_BOX_NEW], [], [Have gtk_box_new function])
AM_COND_IF([ENABLE_GTK2],
           [AC_DEFINE([HAVE_GTK_BOX_NEW], [0], [])],
           [AC_DEFINE([HAVE_GTK_BOX_NEW], [1], [])])

PKG_CHECK_MODULES(GTK, [$GTK_VERSION])
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)


# Checks for header files.
AC_CHECK_HEADERS([iostream \
                  string \
                  sstream \
                  fstream \
                  vector \
                  cstdlib \
                  stdlib.h \
                  sys/time.h \
                  sys/types.h \
                  sys/ipc.h \
                  sys/sem.h \
                  signal.h \
                  limits.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([atexit \
                gettimeofday \
                setenv \
                signal \
                kill \
                semget \
                semctl \
                ftok \
                readlink \
                fork \
                execl])

# Checks for host
AC_CANONICAL_HOST
case $host_os in
    *linux* | *Linux*)
        AC_DEFINE([PROC_SELF_PATH], ["/proc/self/exe"], [Path to symlink in proc that leads to current executable.])
        ;;
    *bsd* | *BSD*)
        AC_DEFINE([PROC_SELF_PATH], ["/proc/curproc/file"], [Path to symlink in proc that leads to current executable.])
        ;;
    *)
        AC_MSG_ERROR([Your platform $host_os is not currently supported.
                      Please, report this to the issue tracker.])
        ;;
esac

# Flags
AM_CPPFLAGS="-Wall"
AM_LDFLAGS="-Wl,--as-needed"
AC_SUBST(AM_CPPFLAGS)
AC_SUBST(AM_LDFLAGS)

AC_CONFIG_FILES([Makefile
                 doc/Makefile
                 src/Makefile])

AC_OUTPUT
