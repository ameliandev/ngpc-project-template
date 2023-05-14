#!/bin/bash

# *****************************************************
# *                   CONSTS                          *
# *                                                   *
# *     Configure the 'optional' constants first      *
# *****************************************************

# (1==true, 0==false) Resize Rom converts the rom size to 2Mb for use in flash carts. This is because flash carts only allow roms bigger than 2Mb. Not needed if your rom is bigger. 
ResizeRom=1
# (1==true, 0==false) Run allows to run the compiled rom through the established emulator.
Run=0
# The ngc file name. If you want to change the cartridge rom name, change it on CartTitle var into carthdr.h file.
romName='main'
compilerPath=/home/[username]/ngpcbins/T900

# *****************************************************
# *                 EMULATOR CONST                    *
# *****************************************************
# corePath='/home/[username]/snap/retroarch/current/.config/retroarch/cores'
# compileSuccess=1
# error=0
# emu_neopop='race_libretro.so'
# emu_mednafe='mednafen_ngp_libretro.so'


# *****************************************************
# *                 COMPILER CONST                    *
# *****************************************************
romNameOut=$romName'.ngc'


# Environment paths & vars
if [[ ! $PATH =~ ^$compilerPath ]]; then
    export PATH=$PATH:$compilerPath/BIN/
    export THOME=$compilerPath
fi

# compiling
make clean
make
make move_files

if [ "$ResizeRom" -eq 1 ]; then
    resizeCmd="py ./utils/NGPRomResize.py ./bin/$romNameOut"
    $resizeCmd
    rm "./bin/$romNameOut"
    mv "./utils/2MB_$romNameOut" "./bin/$romNameOut"
fi

if [ "$Run" -eq 1 ]; then
    retroArchCmd='retroarch --libretro '$corePath'/'$emu_mednafe' ./bin/'$romNameOut
    $retroArchCmd
fi
