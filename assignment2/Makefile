obj-m:=dev_list.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	gcc user_prog.c -o user_prog
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm dev_list
