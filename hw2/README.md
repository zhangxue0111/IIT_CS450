# Lab 2:  System Call

## 1. Boot xv6

Fetch the xv6 source for the lab

```
git clone git://github.com/mit-pdos/xv6-public.git
```

Build and run xv6

```
$ make
+ as kern/entry.S
+ cc kern/entrypgdir.c
+ cc kern/init.c
+ cc kern/console.c
+ cc kern/monitor.c
+ cc kern/printf.c
+ cc kern/kdebug.c
+ cc lib/printfmt.c
+ cc lib/readline.c
+ cc lib/string.c
+ ld obj/kern/kernel
+ as boot/boot.S
+ cc -Os boot/main.c
+ ld boot/boot
boot block is 380 bytes (max 510)
+ mk obj/kern/kernel.img
```

```
$ make qemu
cpu1: starting 1
cpu0: starting 0
sb: size 1000 nblocks 941 ninodes 200 nlog 30 logstart 2 inodestart 32 bmap start 58
init: starting sh
$ 
```

## 2. Trace `close()` system call

For the story of description the execution of `close()`system call, please see  **Trace_Close.pdf** for detail.

## 3. Implementation of `countTrap()` system call

1. the design of system call countTraps(), please see  **countTraps_implementation.pdf**
2. for the changes, build and execute, see **build_execute.pdf**

## 4. Testcases description

see detail in **testcase.pdf**

## 5. source, executables

see **xv6-public.zip**. If you have successfully installed xv6 and already boot it like the above description, then just unzip the xv6-public, `cd xv6-public` . This directory contains the executables files and changes to implement this task.

