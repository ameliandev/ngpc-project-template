@echo off

cls

REM *****************************************************
REM *                   CONSTS                          *
REM *                                                   *
REM *     Configure the 'optional' constants first      *
REM *****************************************************

REM (1==true, 0==false) Resize Rom converts the rom size to 2Mb for use in flash carts. This is because flash carts only allow roms bigger than 2Mb. Not needed if your rom is bigger. 
SET ResizeRom=1
REM (1==true, 0==false) Run allows to run the compiled rom through the established emulator.
SET Run=1
REM The ngc file name. If you want to change the cartridge rom name, change it on CartTitle var into carthdr.h file.
SET romName=main
REM Your ngpcbins\T900 compiler folder. By default
SET compilerPath=C:\ngpcbins\T900
REM Emulator path. If you put Run const equals 1, this var value must exists
SET emuPath="%NGPEMU%\NeoPop\NeoPop-Win32.exe"
@REM SET emuPath="%NGPEMU%\BizHawk-2.2.1\EmuHawk.exe"
@REM SET emuPath="%NGPEMU%\ares-v122\lucia.exe"


REM *****************************************************
REM *                 COMPILER CONST                    *
REM *****************************************************

SET THOME=%compilerPath%
SET BinPath=bin
SET rootPath=%~dp0


REM *****************************************************
REM *                  COMPILATION                      *
REM *****************************************************

path=%path%;%THOME%\bin

make clean
make
make move_files

if "%ResizeRom%"=="1" (
    MOVE "%~dp0%BinPath%\%romName%.ngc" "%~dp0%BinPath%\_%romName%.ngc" > nul
    "%~dp0utils\NGPRomResize.exe" "%~dp0%BinPath%\_%romName%.ngc" > nul
    MOVE "%~dp0%BinPath%\_%romName%.ngc" "%~dp0%BinPath%\%romName%.ngc" > nul
    DEL "%~dp0%BinPath%\2MB__%romName%.ngc" > nul
)

if "%Run%"=="1" (
    %emuPath% %rootPath:~0,-1%.\%BinPath%\%romName%.ngc
)