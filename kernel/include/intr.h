#ifndef INTR_H
#define INTR_H

#include <stdint.h>
#include "multiboot.h"

typedef void (*func_ptr)(void);

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

struct multiboot_info *pmb_info;

void init_gdt(void);
void init_intr(void);

struct cpu_state* handle_interrupt(struct cpu_state* cpu);
void register_interrupt_handler(uint8_t n, func_ptr handler);

#endif
