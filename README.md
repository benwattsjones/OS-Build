Operating System
================
A simple operating system featuring... Heavily commented and hoped to be 
helpfull to others attempting to build an OS...
Dependencies:
-------------
[gcc, nasm, qemu, cross-compiler?, to create 32-bit asm from gcc on arch need
lib32-gcc-libs and to enable multilib]

Building and running:
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

Progression goals:
------------------
 - Standalone bootloader allows OS to boot from ISO CD-ROM. _[DONE]_
 - Kernel runs in 32bit protected mode with C. _[DONE]_
 - Text mode screen driver. _[DONE]_
 - Hardware and software interrupts handling. _[DONE]_
 - System timer driver. _[DONE]_
 - Keyboard driver.
 - Cross-compiler.
 - Physical memory manager.
 - Virtual memory manager with paging.
 - Hierarchical file system with basic text editor.
 - Heap and stack allocator with program loader.
 - User land.
 - Process management with multithreading.
 - Basic graphics (mainly textual, but in sections).

Usefull resources:
------------------
https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf
https://www.cs.bham.ac.uk/~exr/lectures/opsys/15_16/lectures.php
http://wiki.osdev.org/Tutorials
https://littleosbook.github.io/
https://github.com/iankronquist/kernel-of-truth
https://software.intel.com/sites/default/files/managed/a4/60/325384-sdm-vol-3abcd.pdf
http://www.brokenthorn.com/Resources/
https://github.com/cfenollosa/os-tutorial

License:
--------
This software is licensed under the GNU GPLv3 license (see 'LICENSE'). 
It is hoped The source code will be usefull to others attempting to build their 
own simple operating systems.

This project is largely an educational one for myself. As such, whilst bug 
fixes and links to your own extensions are welcome, I am not ready to accept
any major extensions to this work.

