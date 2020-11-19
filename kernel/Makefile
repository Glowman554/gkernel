SRCS = $(shell find -name '*.[cSn]')
OBJS = $(addsuffix .o,$(basename $(SRCS)))

CC = gcc
LD = ld

ASFLAGS = -m32
CFLAGS = -m32 -Wall -g -fno-stack-protector -I include -Wno-incompatible-pointer-types -Wno-int-conversion -Wno-unused-variable -Wno-return-type
LDFLAGS = -melf_i386 -Tkernel.ld

kernel: $(OBJS)
	@echo LD $^
	@$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	@echo CC $^
	@$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.S
	@echo AS $^
	@$(CC) $(ASFLAGS) -c -o $@ $^

%.o: %.n
	@echo NASM $^
	@nasm -f elf32 -o $@ $^

clean:
	rm $(OBJS)

.PHONY: clean
