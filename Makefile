all:
	cd src && make
	cp src/kvs.ko out/
	gcc -std=c99 -Wall -I src/ test/kvs_test.c -o out/kvs_test.o
	gcc -std=c99 -Wall -I src/ test/kvs_test_put.c -o out/kvs_test_put.o

clean:
	cd src && make clean
	rm out/*
