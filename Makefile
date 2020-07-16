all: kernel

kernel: config
	$(MAKE) -C kernel

config:
	python3 config.py

push:
	$(MAKE) -C kernel clean
	git add .
	git commit -m "autopush"
	git push -u origin master

.PHONY: all kernel config push

