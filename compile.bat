@echo off

cls

::Var declarations
SET romName=main
SET rootPath=%~dp0
SET THOME=C:\ngpcbins\T900
SET emuPath="%NGPEMU%\BizHawk-2.2.1\EmuHawk.exe"
@REM set emuPath="%NGPEMU%\NeoPop\NeoPop-Win32.exe"
@REM set emuPath="%NGPEMU%\ares-v122\lucia.exe"

path=%path%;C:\ngpcbins\T900\bin

::Borrado previo--------------------------------------------------------------------
SET list=ngp ngc rel abs s24 map sav 
(for %%a in (%list%) do ( 
    if exist .\bin\*.%%a del .\bin\*.%%a > nul
))

del /S *.rel > nul

make

::Movemos binarios------------------------------------------------------------------
if exist %romName%.ngp rename %romName%.ngp %romName%.ngc
(for %%a in (%list%) do ( 
    if exist %romName%.%%a move %romName%.%%a ".\bin\%romName%.%%a" > nul
))

::Ejecutamos------------------------------------------------------------------------
%emuPath% %rootPath:~0,-1%.\bin\%romName%.ngc