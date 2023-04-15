### COMPILATION ON WINDOWS

- Extract install\ngpcbins_x86_x64.zip for Windows 7 or later, or install\ngpcbins_winXP.zip for Windows XP, to C:\ngpcbins\
- Install [cygwin](https://www.cygwin.com/install.html) in C:\cygwin64. During the installation process, in the package selection step, search for the package 'make' and install it, as it is required.
- If you want to run the ROM on an emulator after compilation, edit line 9 of the **compile.bat** file.
- In VSCode, open the project and execute the compile.bat file.

### COMPILATION ON LINUX

It is possible to compile on Linux using **Wine** and the T900 chip compilers from Windows. Follow these steps:

- Extract install/ngpcbins_x86_x64.zip to a convenient location, for example /home/[user]/ngpcbins
- Check that the directory /T900/BIN exists within this directory, with the common .exe and .dll files of the compiler.
- Rename the makefile for the Linux operating system, i.e., rename makefile_linux to makefile

```bash
mv makefile makefile_old #if you already have one, rename it as _old
mv makeflie_linux makefile
```

- Configure some vars like userName and romname in compile.sh

- You can (if you want, it's not necessary) include the necessary environment variables in your system. To do so, execute the following lines in the console or add them at the **end of your ~/.bashrc file**. If you don't include them, the compiler.sh file for compiling on Linux includes them in case they don't exist.

```bash
export PATH=$PATH:/home/[user]/ngpcbins/T900/BIN/
export THOME=/home/[user]/ngpcbins/T900/
```

- Please verify that the current makefile file contains the execution of commands with .exe files, using Wine with the $THOME path, for example:

```make
$(NAME).ngc: makefile ngpc.lcf $(OBJS)
	wine $(THOME)/BIN/tulink.exe -la -o $(NAME).abs ngpc.lcf system.lib $(OBJS)
	wine $(THOME)/BIN/tuconv.exe -Fs24 $(NAME).abs
	wine $(THOME)/BIN/s242ngp.exe $(NAME).s24

.c.rel:
	wine $(THOME)/BIN/cc900.exe -c -O3 $< -o $@
```

- Now run the following commands in the terminal, within the project directory:

```bash
bash compile.sh
```

### USAGE OF COLORS IN NGPC

There is a maximum of 3 colors per Tile. The 4th color is represented as 0, and the framework does not evaluate it, treating it as direct transparency.

#### FORMATO

```
0x0000 = 0x[SUFFIX]
```

##### SUFFIX

Each value of the suffix represents two cells, resulting in 8 pixels for 4 suffix values multiplied by 2. Each position represents an odd or even value, depending on the sum of the cells it represents, resulting in a different value.

```
Color 1 = Odd 4, Even 1
Color 2 = Odd 8, Even 2
Color 3 = Odd c, Even 3
```

#### EXPLANATION

- 1st digit = Cells 1 and 2
- 2nd digit = Cells 3 and 4
- 3rd digit = Cells 5 and 6
- 4th digit = Cells 7 and 8
