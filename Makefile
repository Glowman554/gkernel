all: kernel programs

kernel:
	$(MAKE) -C kernel

programs:
	$(MAKE) -C programs

iso:
	cp ./kernel/kernel ./cdrom_files
	cp ./programs/terminal/terminal.bin ./initrd/files/.
	cp ./programs/writer/writer.bin ./initrd/files/.
	cp ./programs/init/init.bin ./initrd/files/.
	$(MAKE) -C initrd
	cp ./initrd/initrd.img ./cdrom_files/.
	mkisofs -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o cdrom.iso cdrom_files/

.PHONY: all kernel programs iso

