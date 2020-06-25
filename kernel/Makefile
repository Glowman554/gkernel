SRCS = $(shell find -name '*.[cSn]')
OBJS = $(addsuffix .o,$(basename $(SRCS)))

CC = gcc
LD = ld

ASFLAGS = -m32
CFLAGS = -m32 -Wall -g -fno-stack-protector -I include -Wno-incompatible-pointer-types -Wno-int-conversion -Wno-unused-variable -Wno-return-type
LDFLAGS = -melf_i386 -Tkernel.ld

kernel: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.S
	$(CC) $(ASFLAGS) -c -o $@ $^

%.o: %.n
	nasm -f elf32 -o $@ $^

clean:
	rm $(OBJS)

.PHONY: clean
