cd cdrom_files
rm terminal.bin
rm test.bin
rm kernel
cd ..
cp ./kernel/kernel ./cdrom_files
cp ./programs/terminal/terminal.bin ./cdrom_files
cp ./programs/test/test.bin ./cdrom_files

mkisofs -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o cdrom.iso cdrom_files/
