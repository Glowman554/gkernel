cd cdrom_files
rm kernel
rm initrd.img
cd ..

cd initrd
rm *.bin
cd ..

cp ./kernel/kernel ./cdrom_files
cp ./programs/terminal/terminal.bin ./initrd/files
cp ./programs/writer/writer.bin ./initrd/files

cd initrd 
python build.py
cd ..


cp ./initrd/initrd.img ./cdrom_files
mkisofs -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o cdrom.iso cdrom_files/
