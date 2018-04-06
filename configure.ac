#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_LANG_PUSH([C])
AC_INIT([moolticute-pass], [0.0.1])
AC_CONFIG_AUX_DIR([.])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AM_SILENT_RULES([yes])
LT_INIT
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
])
AC_CONFIG_SUBDIRS([libmoolticute-c])

# Checks for programs.
AC_PROG_CC

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h string.h stdio.h ])


# Check for required libraries of moolticute-c
AC_SEARCH_LIBS([pthread_create], [pthread], [], [
  AC_MSG_ERROR([unable to find pthread library])
])

AC_SEARCH_LIBS([json_object_new_object], [json-c], [], [
  AC_MSG_ERROR([unable to find json-c library])
])

AC_SEARCH_LIBS([lws_create_context], [websockets], [], [
  AC_MSG_ERROR([unable to find websockets library])
])


# create an commandline option to activate/deactivate X11 usage
AC_ARG_ENABLE([x11-clipboard],
  [AS_HELP_STRING([--enable-x11-clipboard],
    [support copying password to X11 clipboard @<:@default=check@:>@])],
  [],
  [enable_x11_clipboard=check])

AS_CASE(
  ["$enable_x11_clipboard"],
  [yes], [AC_SEARCH_LIBS([XInternAtom], [X11],   [HAVE_X11=1])],
  [no], [],
  [AC_SEARCH_LIBS( [XInternAtom], [X11],  [HAVE_X11=1])]
)
AM_CONDITIONAL(HAVE_X11, [test "$enable_x11_clipboard" != no -a "$HAVE_X11" -eq 1])
AS_IF([test "$enable_x11_clipboard" != no -a "$HAVE_X11" -eq 1], [AC_DEFINE(HAVE_X11,[1],[Set if X11 library is available])])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset strtol])

AC_OUTPUT
