all: kernel

kernel: config
	$(MAKE) -C kernel

config:
	python3 config.py

push:
	git add .
	git commit -m "autopush"
	git remote add origin https://github.com/Glowman554/gkernel.git
	git push -u origin master

.PHONY: all kernel config push

