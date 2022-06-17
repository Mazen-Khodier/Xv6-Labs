// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct kmem {
  struct spinlock lock;
  struct run *freelist;
} kmems[NCPU];

int
get_cpu_id()
{
  push_off();
  int cpu_id = cpuid();
  pop_off();
  return cpu_id;
}

void
kinit()
{
  char str[7];
  for(int i = 0; i < NCPU; i++){
    snprintf(str, 7, "kmem_%d", i);
    initlock(&kmems[i].lock, str);
  }
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;
  int cpu_n = get_cpu_id();

  acquire(&kmems[cpu_n].lock);
  r->next = kmems[cpu_n].freelist;
  kmems[cpu_n].freelist = r;
  release(&kmems[cpu_n].lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;
  int cpu_n = get_cpu_id();
  
  for(int i = 0; i < NCPU; i++){
    int lock_n = (cpu_n + i) % NCPU;
    acquire(&kmems[lock_n].lock);
    r = kmems[lock_n].freelist;
    if(r){
      kmems[lock_n].freelist = r->next;
      release(&kmems[lock_n].lock);
      break;
      }
    release(&kmems[lock_n].lock);
  }
  
  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
