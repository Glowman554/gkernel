all: kernel programs

kernel:
	$(MAKE) -C kernel

programs:
	$(MAKE) -C programs

.PHONY: all kernel programs

