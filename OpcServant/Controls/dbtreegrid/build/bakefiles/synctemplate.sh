#!/bin/sh
#
# Author: Francesco Montorsi
# RCS-ID: $Id: synctemplate.sh,v 1.2 2005/11/21 22:51:20 frm Exp $
#
# Keeps the files of this folder synchronized with the files of the wxCode/template
# folder (empty.bkl.template, Bakefiles.bkgen.template, etc)

cp -f empty.bkl.template ../../template/build/MYCOMP.bkl
cp -f Bakefiles.bkgen.template ../../template/build/Bakefiles.bkgen
cp -f ../autoconf/configure.ac ../../template/build
cp -f ../autoconf/acregen.sh ../../template/build
cp -f ../autoconf/config.* ../../template/build
cp -f ../autoconf/install.sh ../../template/build
