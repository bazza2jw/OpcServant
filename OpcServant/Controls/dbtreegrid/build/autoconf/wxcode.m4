dnl ---------------------------------------------------------------------------
dnl Author:          Francesco Montorsi
dnl Creation date:   14/9/2005
dnl RCS-ID:          $Id: wxcode.m4,v 1.9 2006/03/13 14:42:48 frm Exp $
dnl Purpose:         Some M4 macros specific for wxCode components
dnl ---------------------------------------------------------------------------



dnl ---------------------------------------------------------------------------
dnl AM_WXCODE_INIT
dnl
dnl Sets as language the C++ language and includes the wxpresets.m4 macro file
dnl ---------------------------------------------------------------------------
AC_DEFUN([AM_WXCODE_INIT],
        [
            AC_PREREQ([2.57])
            AC_CONFIG_AUX_DIR([../../../build/autoconf build])
            m4_include(../../../build/autoconf/wxpresets.m4)
            AC_LANG(C++)
        ])



dnl ---------------------------------------------------------------------------
dnl AM_WXCODE_CHECKS([minimum-wx-version], [addwxlibraries])
dnl
dnl This macro exists only to handle eventually future changes that will affect
dnl AM_WXCODE_CHECKS but will leave AM_WXPRESETS_FULL checks untouched
dnl (i.e. this macro adds an abstraction layer to wxCode configure scripts).
dnl ---------------------------------------------------------------------------
AC_DEFUN([AM_WXCODE_CHECKS],
        [AM_WXPRESETS_FULL([$1], [$2])])



dnl ---------------------------------------------------------------------------
dnl AM_WXCODE_ARG_ENABLE([name of the --enable- feature],
dnl                      [description of that feature],
dnl                      [default value for that feature])
dnl
dnl ---------------------------------------------------------------------------
AC_DEFUN([AM_WXCODE_ARG_ENABLE], [AC_ARG_ENABLE($1,
                        AC_HELP_STRING([--enable-$1],
                        [$2 (default is $3)]),, [enable_$1=$3])])



dnl ---------------------------------------------------------------------------
dnl AM_WXCODE_ARG_WITH([name of the --with- option],
dnl                    [description of that option],
dnl                    [default value for that option])
dnl
dnl ---------------------------------------------------------------------------
AC_DEFUN([AM_WXCODE_ARG_WITH], [AC_ARG_WITH($1,
                        AC_HELP_STRING([--with-$1],
                        [$2 (default is $3)]),, [with_$1=$3])])



dnl ---------------------------------------------------------------------------
dnl AM_WXCODE_END_PART1
dnl
dnl Tells autoconf which output files must be generated; includes bakefile
dnl checks for the options present in the BKL file; prints a nice message
dnl about the configuration of the component.
dnl ---------------------------------------------------------------------------
AC_DEFUN([AM_WXCODE_END_PART1],
        [
            dnl config output files
            AC_BAKEFILE([m4_include(autoconf_inc.m4)])
            AC_CONFIG_FILES([Makefile])
            AC_OUTPUT

            AM_WXPRESETS_MSG_BEGIN
        ])



dnl ---------------------------------------------------------------------------
dnl AM_WXCODE_END_PART2
dnl
dnl Completes the message to the user printing the settings of the wxWidgets
dnl build which will be used to compile the component.
dnl ---------------------------------------------------------------------------
AC_DEFUN([AM_WXCODE_END_PART2],
        [AM_WXPRESETS_MSG_END])




dnl ---------------------------------------------------------------------------
dnl AM_WXCODE_END
dnl
dnl The WXCODE_END macros are divided in two parts AM_WXCODE_END_PART1 and
dnl AM_WXCODE_END_PART2 so that if you want to show additional component
dnl configuration settings you can write:
dnl
dnl AM_WXCODE_END_PART1
dnl if [[ "$MYVAR" = "1" ]]; then
dnl     echo "  - MYVAR is set"
dnl else
dnl     echo "  - MYVAR is not set"
dnl fi
dnl AM_WXCODE_END_PART2
dnl ---------------------------------------------------------------------------
AC_DEFUN([AM_WXCODE_END],
        [
            AM_WXCODE_END_PART1
            AM_WXCODE_END_PART2
        ])


dnl Other generic macros
m4_include(../../../build/autoconf/wxcodecheck.m4)
