#ifndef MM_H
#define MM_H

#include "multiboot.h"

#define NULL ((void*) 0)

void pmm_init(struct multiboot_info* mb_info);
void* pmm_alloc(void);
void pmm_free(void* page);

#define PTE_PRESENT 0x1
#define PTE_WRITE   0x2
#define PTE_USER    0x4

void vmm_init(void);

#endif
