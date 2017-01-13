all:
	cd src && make
	cp src/kvs.ko out/
	gcc -std=c99 -Wall -I src/ test/kvs.c test/kvs_test.c -o out/kvs.o

clean:
	cd src && make clean
	rm out/*
