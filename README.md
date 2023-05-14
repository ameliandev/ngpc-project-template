## DESCRIPTION

A base template for developing software and games for SNK's handheld console, Neo Geo Pocket and Neo Geo Pocket Color.

In addition, this template has been created to develop your games/homebrew for this console, as it offers a quick access to the compilation and execution elements.

It has:

- Compatible compiler for Windows and GNU/Linux.
- Resized to 2Mb roms (to be compatible with flash cartridges)
- Includes the 0.4 framework developed by Ivan Mackintosh in 2003 that facilitates a lot the development with this console, which contains methods to show elements on screen, audio, controls, etc.
- New methods in core directory to save/load games.

## REQUIREMENTS

#### Windows

- Install [cygwin](https://www.cygwin.com/install.html) in a convenient path such as C:\cygwin64. In the installation process, in the **packages** section, **make sure to install the make package**. To do this, in the package finder (top left), select View: Full and in Search: type make. A fairly extensive list will appear, but you should only select the one where the package name says make and its description will say something like **'The GNU version of the 'make' utility'**.

#### GNU/Linux

- In order to compile, you must install the [wine](https://packages.ubuntu.com/search?keywords=wine) package on your GNU/Linux OS.

#### Optional

- **Python**. If you want to increase the rom size by 2Mb if it does not exceed it to test it on your flash cartridge, enable the ResizeRom property in your build file. This process will run a python script, so you must have python installed on your computer. This template has been tested with _python 3.11.1_

## INSTALLATION

On both Windows and Linux, the first thing you must do is to install the compiler for the Toshiba T900 chip. For them you must download the zip file that I link you here according to your operating system and unzip in a path that is easily accessible. On Windows you can use something like **C:/ngpcbins** and on GNU/Linux something like /**home/[user]/ngpcbins**.

- [Compiler for Windows](https://www.dropbox.com/s/iel2jd4gg50zm7b/ngpcbins.zip?dl=0)
- [GNU/Linux compiler](https://www.dropbox.com/s/ky3w4e8ltc7zb16/ngpcbins.tar.gz?dl=0)

## COMPILATION

Before you start you must **set your base makefile**. Currently there are 2 makefile files (_*makefile_win, makefile_linux*_). Depending on the operating system you are going to work with, edit the name of one of them and **leave its name as makefile**, e.g. if you are going to work on windows, rename makefile_win to makefile.

### Windows

To compile on Windows, first edit the **build.bat** file, specifically the OPTIONAL CONSTANTS section.

- ResizeRom: Indicates if the rom should be resized to 2Mb if it does not exceed it.
- Run: Once compiled the game, it executes it in the emulator that you have configured.
- romName: Name of the final file, by default 'main'. If you want to rename the game title in the cartridge header, its name must also be edited in the file _carthdr.h_, **variable CartTitle**.
- compilerPath: Directory where the **compiler binaries of the Toshiba T900 chip are located**, by default C:\ngpcbinsT900
- emuPath: Absolute path to the .exe file of the **emulator** that will run the game once compiled.

Once you have edited these constants, access through console to the address of this template and execute build.bat

### GNU/Linux

To compile in GNU/Linux, first edit the **build.sh** file, specifically the OPTIONAL CONSTANTS section

- ResizeRom: Indicates if the rom should be resized to 2Mb if it does not exceed it. Edit line 45 of your build.sh and replace the name of your python executable with the current one (_resizeCmd="**py** ./utils/NGPRomResize.py ./bin/$romNameOut"_).
- Run: Once the game is compiled, it executes it in the emulator that you have configured.
- romName: Name of the final file, by default 'main'. If you want to rename the game title in the cartridge header, its name must also be edited in the file _./carthdr.h_, **variable CartTitle**.
- compilerPath: Directory where the **compiler binaries of the Toshiba T900 chip are located**, by default /home/[user]/ngpcbins

#### RUN WITH EMULATOR

As in Windows, in GNU/Linux it is possible to execute the rom once compiled, but in this case it differs from Windows, since here the execution by commands varies according to the emulator and the configuration of the system, reason why it is something that you will have to prepare yourself. To do this, edit line 52 of build.sh. Currently it is prepared to run retroArch in particular with the mednafen emulator.
