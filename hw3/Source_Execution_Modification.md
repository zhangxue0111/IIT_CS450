# Source_Execution_Modification

# Overview

The code for PA3. 

If you use your own computer and don't install xv6 system, please see the link https://pdos.csail.mit.edu/6.S081/2021/tools.html to see how to install and get the base source code.  My base source code comes from the `git clone git://g.csail.mit.edu/xv6-labs-2021`

## 1. Source

All sources code based on the `xv6-labs-2021`. To implement a pair of system calls, I made several modifications and added several new functions to the related files. 

## 2. Execution

1. download the submitted source file `xv6-labs-2021.zip`, unzip it.
2. `cd xv6-labs-2021`
3. `make qemu`
4. execute test file `share.c`

## 3. Modification for implementation

### 1. In `proc.h` add`struct shared_page_mapping`  and a related attribute in `proc`

```c
// record mapped shared pages
struct shared_page_mapping {
  int key;
  void *va;
};

// Per-process state
struct proc {
  struct spinlock lock;
    ...
  struct shared_page_mapping shm[32]; // process shared pages
};
```

### 2. Add several functions in `vm.c`

#### 2.1 Add `struct shm_region`

```c
typedef char bool;
struct shm_region {
  bool valid;
  int rc;
  int len;
  uint64 physical_pages[MAX_REGION_SIZE];
};
```

#### 2.2 Add `struct map_shm_region`

```c
void map_shm_region(int key, struct proc *p, void *addr) {
  for (int k = 0; k < regions[key].len; k++) {
    mappages(p->pagetable, (uint64)(addr + (k*PGSIZE)), PGSIZE, regions[key].physical_pages[k], PTE_V|PTE_W|PTE_U|PTE_R);
  }
}
```

#### 2.3 Add `getsharedpage`

```c
void *
getsharedpage(int key, int len)
{
  uint64 mem;
  if(key < 0 || key > 32)
    return (void *)0;

  struct proc *p = myproc();

  // Allocate pages in the appropriate regions' pysical pages
  if(!regions[key].valid) {
    for(int j = 0; j < len; j++) {
      if((mem = (uint64)kalloc()) == 0)
        return (void *)-1;
      memset((void *)mem, 0, PGSIZE); // fill in zero in this page
      regions[key].physical_pages[j] = mem; // Save new page
      //printf("save memory success.\n");
    }
    regions[key].valid = 1;
    regions[key].len = len;
    regions[key].rc = 0;
  }
  regions[key].rc += 1;

  // Find the index in the process
  int shind = -1;
  for (int x = 0; x < 32; x++) {
    if (p->shm[x].key == -1) {
      shind = x;
      break;
    }
  }
  if (shind == -1)
    return (void*)0;

  //printf("shind= %d\n", shind);

  // Get the virtual address space currently allocated
  void *va = (void*)KERNBASE-PGSIZE;
  for (int x = 0; x < 32; x++) {
    if (p->shm[x].key != -1 && (uint64)(va) > (uint64)(p->shm[x].va)) {
      va = p->shm[x].va;
    }
  }

  // Get va of new mapped pages
  va = (void*)va - (len*PGSIZE);
  p->shm[shind].va = va;
  p->shm[shind].key = key;

  p->shm[shind].va = (void*)va;
  p->shm[shind].key = key;

  // Map them in memory
  map_shm_region(key, p, (void*)va);

  //printf("map success.\n");

  return (void*) va;
}
```

#### 2.4 add `freesharedpage`

```c
int
freesharedpage(int key)
{
  // Clear shared memory data structure
  struct proc *p = myproc();
  void *va = 0;
  for (int i = 0; i < 32; i++) {
    if (p->shm[i].key == key) {
      va = p->shm[i].va;
      p->shm[i].key = -1;
      p->shm[i].va = 0;
      break;
    }
  }
  if(va == 0)
    return -1;

  // Clear page table entries for all pages in the process
  struct shm_region* reg = &regions[key];
  for(int i = 0; i < reg->len; i++) {
    pte_t* pte = walk(p->pagetable, (uint64)va + i*PGSIZE, 0);
    if(pte == 0) {
      return -1;
    }
    *pte = 0;
  }

  // Decrease the refcount, freeing if unused.
  reg->rc--;
  if(reg->rc == 0) {
    regions[key].valid = 0;
    regions[key].rc = 0;
    for(int i = 0; i < regions[key].len; i++)
      kfree((void*)(regions[key].physical_pages[i]));
    regions[key].len = 0;
  }

  return 0;
}
```

#### 2.5  add `copy_shared_regions`to deal with fork

we also need to add this function in `proc.c` for `fork` function.

```c
// Copy the shared memory regions of process p into process np
int copy_shared_regions(struct proc *p, struct proc *np) {
  for (int i = 0; i < 32; i++) {
    if (p->shm[i].key != -1) {
      np->shm[i] = p->shm[i];
      // map into the new forked proc
      int key = np->shm[i].key;
      regions[key].rc++;
      map_shm_region(key, np, np->shm[i].va);
    }
  }
  return 0;
}
```

### 3. deal with `usertrap` in `trap.c`

One of the many neat tricks an O/S can play with page table hardware is lazy allocation of user-space heap memory. When the process first tries to use any given page of lazily-allocated memory, the CPU generates a page fault, which the kernel handles by allocating physical memory, zeroing it, and mapping it.  So we need to add a handle to deal with the trap in `usertrap`

```c
else if (r_scause() == 13 || r_scause() == 15){
    // page fault
    uint64 va = r_stval();
    if (va >= p->sz || va < p->trapframe->sp) {
      // page-faults on a virtual memory address higher than any allocated with sbrk()
      // or lower than the stack. In xv6, heap is higher than stack
      p->killed = 1;
    } else {
      uint64 ka = (uint64) kalloc();
      if (ka == 0){
        p->killed = 1;
      } else {
        memset((void *)ka, 0, PGSIZE);
        va = PGROUNDDOWN(va);
        if(mappages(p->pagetable, va, PGSIZE, ka, PTE_W|PTE_R|PTE_U) != 0) {
          kfree((void *)ka);
          p->killed = 1;
        }
      }
    }
```

#### 4. Add `free_possible_physical_page` in `vm.c`

This function is used to deal with the `panic freewalk: leaf` problem. As the mechanism of `sbrk()`and lazy allocation, it may exist that one page may split to two page. So `freesharedpage` may not free that remaining shared page, which lead to the panic.

```c
// Free any pages which fail is_shm_pa (are not shared)
void free_possible_physical_page(void *v) {
  struct proc *p = myproc();
  uint64 pa = walkaddr(p->pagetable, (uint64)v);
  if (!is_shm_page(pa)) 
    kfree((void*) v);
}
```

#### 5. Add $U_share, $U_client to UPROGS in Makefile

```c
UPROGS=\
	$U/_cat\
	$U/_echo\
	$U/_forktest\
	$U/_grep\
	$U/_init\
	$U/_kill\
	$U/_ln\
	$U/_ls\
	$U/_mkdir\
	$U/_rm\
	$U/_sh\
	$U/_stressfs\
	$U/_usertests\
	$U/_grind\
	$U/_wc\
	$U/_zombie\
	$U/_share\
	$U/_client\
```

#### 
