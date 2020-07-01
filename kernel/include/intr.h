#ifndef INTR_H
#define INTR_H

#include <stdint.h>
#include "multiboot.h"

struct cpu_state {
    // Von Hand gesicherte Register
    uint32_t   eax;
    uint32_t   ebx;
    uint32_t   ecx;
    uint32_t   edx;
    uint32_t   esi;
    uint32_t   edi;
    uint32_t   ebp;

    uint32_t   intr;
    uint32_t   error;

    // Von der CPU gesichert
    uint32_t   eip;
    uint32_t   cs;
    uint32_t   eflags;
    uint32_t   esp;
    uint32_t   ss;
};

struct task {
	struct cpu_state*   cpu_state;
	struct task*        next;
	int pid;
};

void init_gdt(void);
void init_intr(void);
void init_multitasking(struct multiboot_info* mb_info);

struct cpu_state* handle_interrupt(struct cpu_state* cpu);
struct cpu_state* schedule(struct cpu_state* cpu);
struct task* init_task(void* entry);
void init_elf(void* image);
void kexit(int code);
int getproccount();
#endif
