#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([yamesher], [0.1.0], [shabbychef@gmail.com])
# make a few of these:
AC_CONFIG_SRCDIR([types.h])
AC_CONFIG_SRCDIR([geometry.h])
AC_CONFIG_SRCDIR([math.h])

AC_CONFIG_HEADERS([config.h])

# c.f.
# http://stackoverflow.com/a/5302054/164611
# http://www.gnu.org/software/autoconf-archive/ax_lib_cgal_core.html#ax_lib_cgal_core
AC_CONFIG_MACRO_DIR([autoconf])


# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Check CGAL
AX_LIB_CGAL_CORE([AC_MSG_RESULT([CGAL found])],
 [AC_MSG_ERROR([CGAL is required; try libcgal-dev?])])


# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_C_RESTRICT
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_CHECK_FUNCS([floor sqrt])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
