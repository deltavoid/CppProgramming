

.PHONY: build run
default: run


build: main
main: main.c
	gcc -o $@ $^


run: build
	./main

run_strace: build
	strace ./main

ldd: build
	ldd ./main


