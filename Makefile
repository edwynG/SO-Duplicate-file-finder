all: prepare main

prepare:
	mkdir -p obj 

clean:	
	rm -rf obj/*

main: src/main.c
	gcc src/main.c -o obj/main resources/md5-lib/libmd5.a
	./obj/main -t 0 -d string -m c
# -t int -d string -m char