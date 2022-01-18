## Overview

Develop a set of tools that can help to recover a damaged file system.

## Source Code

In the compress files, xv6 is the source code that we implement all functions. At the same time, in the `File_Modification` folder, I already added all important modification files for this assignment. 

In the kernel, we modify the following files:

​	`fs.c`:  implement all functions needed to recover a damaged file system.

​	`syscall.c`: add several necessary system calls to help us handle a serials operations.

In the use program, we create the following new files:

​	`dirWalker.c`: test directory walker function

​	`inodeWalker.c`: test inode walker function

​	`damageInode.c`: test damage inode function

​	`recoverFile.c`: test recover directories and files function

​	`recovertc.c`: add new files and directories for test	 

## Execution

1. Uncompress the `xv6.zip` archive in a new directory.
2. Go to that directory using the terminal
3. `promp> make clean`
4. `promp> make`
5. `promp> make qemu`

## Design and Description

For detail, see `Design_Description.pdf`

## Test Cases

For detail, see `TestData.pdf`