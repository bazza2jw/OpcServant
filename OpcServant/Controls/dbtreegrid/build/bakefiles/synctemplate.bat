@echo off
REM
REM Author: Francesco Montorsi
REM RCS-ID: $Id: synctemplate.bat,v 1.3 2005/11/21 22:51:20 frm Exp $
REM
REM Keeps the files of this folder synchronized with the files of the wxCode\template
REM folder (empty.bkl.template, Bakefiles.bkgen.template, etc)

copy /Y empty.bkl.template ..\..\template\build\MYCOMP.bkl
copy /Y Bakefiles.bkgen.template ..\..\template\build\Bakefiles.bkgen
copy /Y ..\autoconf\configure.ac ..\..\template\build
copy /Y ..\autoconf\acregen.sh ..\..\template\build
copy /Y ..\autoconf\config.* ..\..\template\build
copy /Y ..\autoconf\install.sh ..\..\template\build
