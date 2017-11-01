Operating System
================
A simple operating system intended mainly for educational purposes. The code is
very heavily commented in an attempt to make it easier to understand and 
navigate for others attempting similar projects.

Currently, The user interface is in text mode, with a header line displaying
a welcome message and the system time elapsed, with an area below to type in.

Please see the 'Progression Goals' section below for a list of features 
implemented thus far.

Progression Goals:
------------------
 - Standalone bootloader allows OS to boot from ISO CD-ROM. _[DONE]_
 - Kernel runs in 32bit protected mode with C. _[DONE]_
 - Text mode screen driver. _[DONE]_
 - Hardware and software interrupts handling. _[DONE]_
 - System timer driver. _[DONE]_
 - Keyboard driver. _[DONE]_
 - Cross-compiler. _[DONE]_
 - Physical memory manager.
 - Virtual memory manager with paging.
 - Hierarchical file system with basic text editor.
 - Heap and stack allocator with program loader.
 - User land.
 - Process management with multithreading.
 - Basic graphics (mainly textual, but in sections).

Dependencies:
-------------
In order to compile the operating system, the cross compiler must be built.
Instructions for this are listed under the 'Installing the Cross Compiler' 
title below. You must be using GCC on linux. The NASM assembler is also 
required.

QEMU is required to run the OS in a virtual machiene using the './run' script
created upon compilation. However the 'OS.iso' file also created may be ran 
in other emulators, or indeed on actual hardware if burned to a CD.
Whilst I regularly test the OS on bare metal, I do not reccomend this and you
choose to do so entirely at your own risk.

Building and Running:
---------------------
Create the .iso file with command:
`$ make`
This will make bin directory with binary files, .iso file containing OS, and
run file for running OS in qemu emulator
Run the operating system in the qemu emulator with command:
`$ ./run`
Remove the binary files (placed in bin directory) and .iso file with command:
`$ make clean`
If the make fails, you can clean up the files created with the command:
`$ make fail`

__Either 'make fail' or 'make clean' (as appropriate) must be called before 'make'
is called again.__

Usefull Resources:
------------------
- https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf
- http://wiki.osdev.org/Tutorials
- https://software.intel.com/sites/default/files/managed/a4/60/325384-sdm-vol-3abcd.pdf
- http://www.brokenthorn.com/Resources/
- https://github.com/cfenollosa/os-tutorial

License:
--------
This software is licensed under the GNU GPLv3 license (see 'LICENSE'). 
It is hoped The source code will be usefull to others attempting to build their 
own simple operating systems.

This project is largely an educational one for myself. As such, whilst bug 
fixes and links to your own extensions are welcome, I am not ready to accept
any major extensions to this work.

* * *
* * *

Installing the cross compiler
=============================

Before you Begin:
-----------------
The cross compiler and it's source code will be created in the directory
`$HOME/Src/os-cross-compiler`. This directory will be prepended to $PATH to
allow the compiler to be called. It is assumed that you are already in this
directory with:
```
$ cd $HOME/Src 
$ mkdir os-cross-compiler 
$ cd os-cross-compiler
```
If you wish to use a different directory you will also need to modify the 
Makefile.

It is assumed you have the following programs installed and are working in
a linux environment.
 - wget
 - tar

Much credit for these instructions must go to http://wiki.osdev.org/GCC_Cross-Compiler,
which I have modified here.

Downloading the Prerequisits:
-----------------------------
Binutils provides the assembler, disassembler, linker and other programs.
At the time of writing, the latest version of Binutils is 2.29.1. This is the
version tested to work with the cross compiler, but later versions should also.
We will download this with:
```
$ wget "ftp.gnu.org/gnu/binutils/binutils-2.29.1.tar.gz"
$ tar -xvzf binutils-2.29.1.tar.gz
```

We also need to install gcc (note this is independent of the gcc version
already installed globaly on your system). The version tested to work is
7.2.0. We will download and unpack this as follows:
```
$ wget "ftp.gnu.org/gnu/gcc/gcc-7.2.0/gcc-7.2.0.tar.gz"
$ tar -xvzf gcc-7.2.0.tar.gz
```

GCC itself has several prerequisites. The gcc source code provides a way to 
easily download them from the top level gcc directory as follows:
```
$ cd gcc-7.2.0
$ contrib/download_prerequisites
$ cd ..
```

Preparation:
------------
We will define a few variables as follows:
```
$ export PREFIX="$HOME/Src/os-cross-compiler"
$ export TARGET=i686-elf
$ export PATH="$PREFIX/bin:$PATH"
```
PREFIX is the location where all files of the cross compiler will reside. 
TARGET is set to i686-elf to compile for a 32-bit system.
Adding $PREFIX/bin to $PATH is neccessary to allow the programs created when
installing binutils to be accessable when installing gcc.

Installing Binutils:
--------------------
```
$ cd $HOME/Src/os-cross-compiler
$ mkdir build-binutils
$ cd build_binutils
$ ../binutils-2.29.1/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
$ make
$ make install
$ cd ..
```
'--disable-nls' disables native language support, reducing dependencies and 
compile time.
'--with-sysroot' enables sysroot support.
Note the programs created are located in $PREFIX/bin directory.

Installing GCC:
---------------
In order to install gcc, the binutils binaries previously created must be on
$PATH. This was set up earlier and can be checked using:
`$ which -- $TARGET-as || echo $TARGET-as is not in the PATH`
The following compilations take a while to complete.
```
$ cd $HOME/Src/os-cross-compiler/
$ mkdir build-gcc
$ cd build-gcc
$ ../gcc-7.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
$ make all-gcc
$ make all-target-libgcc
$ make install-gcc
$ make install-target-libgcc
```
--without-headers removes reliance on any C library being present for the target.

Using the Cross-Compiler:
-------------------------
The cross compiler can now be accessed using:
`$ $HOME/Src/os-cross-compiler/bin/i686-elf-gcc`
-gcc can also be replaced with -as and -ld to access the assembler and linker.

Alternatively, the binaries could be added to PATH.

This is covered by the Makefile.
