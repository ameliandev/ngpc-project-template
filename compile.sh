#!/bin/bash

# *****************************************************
# *                   CONSTS                          *
# *                                                   *
# *     Configure the 'optional' constants first      *
# *****************************************************

# (1==true, 0==false) Resize Rom converts the rom size to 2Mb for use in flash carts. This is because flash carts only allow roms bigger than 2Mb. Not needed if your rom is bigger. 
ResizeRom=1
# (1==true, 0==false) Run allows to run the compiled rom through the established emulator.
Run=1
# The ngc file name. If you want to change the cartridge rom name, change it on CartTitle var into carthdr.h file.
romName='main'

corePath='/home/[username]/snap/retroarch/current/.config/retroarch/cores'
compileSuccess=1
error=0
emu_neopop='race_libretro.so'
emu_mednafe='mednafen_ngp_libretro.so'

# *****************************************************
# *                 COMPILER CONST                    *
# *****************************************************
romNameOut=$romName'.ngc'
compilerPath=/home/[username]/ngpcbins/T900

# Environment paths & vars
if [[ ! $PATH =~ ^$compilerPath ]]; then
    export PATH=$PATH:$compilerPath/BIN/
    export THOME=$compilerPath
fi

# removing all .rel files
# find . -name "*.rel" | xargs rm 

# cleaning ./bin directory
# rm -r ./bin/* &> /dev/null

# compiling
make clean
make
make move_files

# on error, exit
if [ ! -e $romName'.ngp' ]; then
    echo Error Compilin1g
    exit
fi

if [ "$Run" -eq 1 ]; then
    retroArchCmd='retroarch --libretro '$corePath'/'$emu_mednafe' ./bin/'$romNameOut
    $retroArchCmd
fi

# moving compiled files to ./bin directory
# for file in "$arg"./{$romName'.map',$romName'.abs',$romName'.s24',$romName.'ngp',$romName'.ngc',$romName.'rel'} ; do
#     if [ -e $file ]; then
#         if [ $file = "./"$romName'.ngp' ]; then
#             mv $file ./bin/$romName.'ngc'
#         else
#             mv $file ./bin/$file
#         fi
#     fi
# done

# launching rom on emulator

