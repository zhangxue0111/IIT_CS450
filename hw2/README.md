# Overview

Code for lab 2 assignment.

## 1. Preparation

Fetch the xv6 source for the lab

```
git clone git://g.csail.mit.edu/xv6-labs-2021
```

Build and run xv6:

```
$ make qemu
qemu-system-riscv64 -machine virt -bios none -kernel kernel/kernel -m 128M -smp 3 -nographic -drive file=fs.img,if=none,format=raw,id=x0 -device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0

xv6 kernel is booting

hart 1 starting
hart 2 starting
init: starting sh
$ 
```

To quit qemu type: Ctrl-a x.

## 2. Trace the `close()` system call

The first task is to trace the `close()` system call to figure out what happened when a non-existing file descriptor is given to it to understand the control mechanism of an operation system. I implemented a Unix program `demo` for xv6. If the user puts a non-existing file descriptor, `demo` will print an error message. The code is in the file `user/demo.c` 

