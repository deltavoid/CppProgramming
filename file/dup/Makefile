


build: single_fd dup

single_fd: single_fd.c
	gcc -o $@ $^ -pthread

dup: dup.c
	gcc -o $@ $^ -pthread



# run: run_dup

run_dup: build
	./dup

run_dup_strace: build
	strace ./dup

run_single_fd: build
	./single_fd

