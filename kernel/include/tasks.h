#ifndef TASKS_H
#define TASKS_H

#include "intr.h"
#include "multiboot.h"

struct task {
	struct cpu_state*   cpu_state;
	struct task*        next;
	int pid;
};

void init_multitasking(struct multiboot_info* mb_info);
struct cpu_state* schedule(struct cpu_state* cpu);
struct task* init_task(void* entry);
void init_elf(void* image);
void kexit(int code);
int getproccount();

#endif