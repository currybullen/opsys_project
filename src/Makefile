obj-m += kvs.o
kvs-objs := entrypoint.o kvs_ht.o kvs_file_helper.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
