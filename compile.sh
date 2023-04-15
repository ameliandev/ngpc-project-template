#!/bin/bash

# vars declaration
corePath='/home/[username]/snap/retroarch/current/.config/retroarch/cores'
compileSuccess=1
error=0
emu_neopop='race_libretro.so'
emu_mednafe='mednafen_ngp_libretro.so'
romName='main'
romNameOut=$romName'.ngc'
compilerPath=/home/[username]/ngpcbins/T900

# Environment paths & vars
if [[ ! $PATH =~ ^$compilerPath ]]; then
    export PATH=$PATH:$compilerPath/BIN/
    export THOME=$compilerPath
fi

# removing all .rel files
find . -name "*.rel" | xargs rm 

# cleaning ./bin directory
rm -r ./bin/* &> /dev/null

# compiling
make

# on error, exit
if [ ! -e $romName'.ngp' ]; then
    echo Error Compilin1g
    exit
fi

# moving compiled files to ./bin directory
for file in "$arg"./{$romName'.map',$romName'.abs',$romName'.s24',$romName.'ngp',$romName'.ngc',$romName.'rel'} ; do
    if [ -e $file ]; then
        if [ $file = "./"$romName'.ngp' ]; then
            mv $file ./bin/$romName.'ngc'
        else
            mv $file ./bin/$file
        fi
    fi
done

# launching rom on emulator
retroArchCmd='retroarch --libretro '$corePath'/'$emu_mednafe' ./bin/'$romNameOut
$retroArchCmd
