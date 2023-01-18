AC_PREREQ(2.50)

AC_INIT(automato, 0.0.1, keshav.gupta@uwaterloo.ca)

AC_CONFIG_AUX_DIR(config)

AM_INIT_AUTOMAKE([subdir-objects])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CONFIG_FILES([Makefile])
AC_CHECK_LIB(readline, readline, [echo "readline dependency found"], [sudo apt-get install libreadline-dev])
AC_OUTPUT

SRC_DIR="src"
BUILD_DIR="build"