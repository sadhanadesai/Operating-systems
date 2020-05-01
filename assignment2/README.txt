Sadhana Vilas Desai
B00811705
sdesai13@binghamton.edu

---------------------------------------------
## Description 

a kernel module that creates a /dev/process_list character device. The character device supports the read() operation only.
kernel module returns the following information for all currently running processes:

1.process ID (PID)
2.parent process ID (PPID)
3.the CPU on which the process is running (CPU)
4.its current state (STATE)

---------------------------------------------
## To run:

1. make
2. sudo insmod dev_list.ko  (using root user)
To run kernel space and see process information: dmesg -c
To run user space program and print process list:
3. sudo ./user_prog (using root user)


For checking device file:
ls -l /dev |grep dev_list