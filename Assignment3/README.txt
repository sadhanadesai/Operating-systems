Sadhana Desai
B00811705
sdesai13@binghamton.edu

---------------------------------------------
## Description 

Folder contents:
1.Probe file: main.c
2.User space program: user.c
3.Report with plots and details
4.Makefile to run code
5.README file

main.c
kernel module that takes the process-ID of an active process as an argument(using module parameter)and tracks all the virtual addresses on which the target process faults and print the virtual addresses

User.c
User space program.

Report
It has all the details, graph and observations of the three applications sys bench, perf and kernel compilation.

---------------------------------------------
## To run:

make
sudo insmod main.ko pr_id=<pid>    //to insert module
sudo dmesg                         //to read the logs

gcc user.c -o user                 //to compile and run user space
./user 


sudo dmesg -c                      //to clear log
Sudo rmmod main                    //to remove module