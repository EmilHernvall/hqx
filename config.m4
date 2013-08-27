dnl $Id$
dnl config.m4 for extension hqx

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(hqx, for hqx support,
[  --with-hqx             Include hqx support])

if test -z "$PHP_GD"; then
  PHP_ARG_WITH(gd, for the location of gd,
  [  --with-gd[=DIR]     hqx], no, no)
fi

if test "$PHP_GD" = "yes"; then
  GD_MODULE_TYPE=builtin
else

 if test "$PHP_GD" != "no"; then
  GD_MODULE_TYPE=external

dnl Header path
  for i in include/gd include gd ""; do
    test -f "$PHP_GD/$i/gd.h" && GD_INCLUDE="$PHP_GD/$i"
  done

  if test -z "$GD_INCLUDE"; then
    AC_MSG_ERROR([Unable to find gd.h anywhere under $PHP_GD])
  fi
 fi
fi

if test "$PHP_HQX" != "no"; then
  if test "$PHP_GD" = "no"; then
    AC_MSG_ERROR([HQX extension requires GD extension, add --with-gd])
  fi
  dnl Write more examples of tests here...

  dnl # --with-hqx -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/hqx.h"  # you most likely want to change this
  dnl if test -r $PHP_HQX/$SEARCH_FOR; then # path given as parameter
  dnl   HQX_DIR=$PHP_HQX
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for hqx files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       HQX_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$HQX_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the hqx distribution])
  dnl fi

  dnl # --with-hqx -> add include path
  dnl PHP_ADD_INCLUDE($HQX_DIR/include)

  # --with-hqx -> check for lib and symbol presence
  LIBNAME=hqx # you may want to change this
  LIBSYMBOL=hq4x_32 # you most likely want to change this 

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
  PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $HQX_DIR/lib, HQX_SHARED_LIBADD)
  AC_DEFINE(HAVE_HQXLIB,1,[ ])
  ],[
  AC_MSG_ERROR([wrong hqx lib version or lib not found])
  ],[
  -L$HQX_DIR/lib -lm
  ])

  PHP_SUBST(HQX_SHARED_LIBADD)

  if test "$GD_MODULE_TYPE" = "builtin"; then 
    PHP_ADD_BUILD_DIR($ext_builddir/libgd)
    GDLIB_CFLAGS="-I$ext_srcdir/libgd $GDLIB_CFLAGS"
    GD_HEADER_DIRS="ext/gd/ ext/gd/libgd/"

    PHP_TEST_BUILD(foobar, [], [
      AC_MSG_ERROR([GD build test failed. Please check the config.log for details.])
    ], [ $GD_SHARED_LIBADD ], [char foobar () {}])
  else
    GD_HEADER_DIRS="ext/gd/"
    GDLIB_CFLAGS="-I$GD_INCLUDE $GDLIB_CFLAGS"
    PHP_ADD_INCLUDE($GD_INCLUDE)
    PHP_CHECK_LIBRARY(gd, gdImageCreate, [], [
      AC_MSG_ERROR([GD build test failed. Please check the config.log for details.])
    ], [ $GD_SHARED_LIBADD ])
  fi

  PHP_INSTALL_HEADERS([$GD_HEADER_DIRS])

  dnl PHP_NEW_EXTENSION(hqx, hqx.c, $ext_shared)
  PHP_NEW_EXTENSION(hqx, hqx.c, $ext_shared,, \\$(GDLIB_CFLAGS))
  PHP_ADD_EXTENSION_DEP(hqx, gd)

fi
