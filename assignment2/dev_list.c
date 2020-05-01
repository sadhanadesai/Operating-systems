//
//  main.c
//  Operating_sys2
//
//  Created by Sadhana Desai on 3/15/20.
//  Copyright © 2020 Sadhana Desai. All rights reserved.
//

//** HEADER FILES **//
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/sched.h>
#include<linux/slab.h>
#include<linux/kernel.h>
#include<linux/sched/task.h>
#include<linux/sched/prio.h>
#include<linux/sched/signal.h>
#include<linux/sched/types.h>
#include<linux/signal_types.h>
#include<linux/uaccess.h>
#include<linux/mm_types_task.h>

//** DEFINITIONS **//
#define BUFFER_SIZE 300
#define DEVICE_NAME "dev_list"

//** FUNCTION DECLARATIONS **//
static int open_dev_list(struct inode*,struct file *);
static ssize_t read_dev_list(struct file *file, char __user * out, size_t size, loff_t * off);
static int close_dev_list(struct inode*, struct file *);
static char* Task_State(long);



//** File operations **//

static struct file_operations dev_list_fops = {
    .owner =THIS_MODULE,
    .open = open_dev_list,
    .read = read_dev_list,
    .release = close_dev_list
};

//** Current pointer to task_struct **//
static struct task_struct * curr;
//** majornumber **//
static int register_status;

//** register the device driver **//
static int __init dev_list_init(void){
    printk(KERN_INFO "Process List Initialization KO\n");
    register_status = register_chrdev(0, DEVICE_NAME, &dev_list_fops);
    if(register_status<0){
        printk(KERN_ALERT "Could not register!");
        return 0;
    }
    curr=next_task(&init_task);
    return 0;
}

//** Deregister the device driver **//
static void __exit dev_list_exit(void) {
    printk(KERN_INFO "Process List Exit KO\n");
    unregister_chrdev(register_status, DEVICE_NAME);
}

//** Read function **//
static ssize_t read_dev_list(struct file *file, char *out, size_t size, loff_t * off)
{
    int status;
    struct task_struct * task;
    char proc[BUFFER_SIZE];
    
    memset(proc,0,sizeof(char)*BUFFER_SIZE);
    printk(KERN_INFO "Read Process information:\n");

    for_each_process(task) {
        if(curr==task){
            char* state=Task_State(task->state);
            memset(proc,0,sizeof(char)*BUFFER_SIZE);
            sprintf(proc, "PID=%d PPID=%d CPU=%d STATE=%s",task->pid,task->parent->pid,task_cpu(task),state);
            printk(KERN_INFO "GETTING: %s",proc);
            status = copy_to_user(out, proc, strlen(proc));
            if(status !=0)
            {
                printk(KERN_ALERT "ERROR IN FETCHING DATA");
                return -EFAULT;
            }
            curr=next_task(task);
            break;
        }
    }
    return strlen(proc);
}

//** Open device file **//
static int open_dev_list(struct inode * inode,struct file * file) {
    printk(KERN_INFO "Device file Opened\n");
    curr= next_task(&init_task); //start from first pos
    return 0;
}

//** Close device file **//
static int close_dev_list(struct inode * inode,struct file *file) {
    printk(KERN_INFO "Process_List: Device Closed!\n");
    curr = &init_task;  //reset to first position
    return 0;
}

//** Function to represent task state**//
char* Task_State(long state){
    switch(state) {
        case 0:
            return "TASK_RUNNING";
        case 1:
            return "TASK_INTERRUPTIBLE";
        case 2:
            return "TASK_UNINTERRUPTIBLE";
        case 3:
            return "TASK_INTERRUPTIBLE , TASK_UNINTERRUPTIBLE";
        case 4:
            return "__TASK_STOPPED";
        case 8:
            return "__TASK_TRACED";
        case 15:
            return "TASK_NORMAL , __TASK_STOPPED , __TASK_TRACED";
        case 16:
            return "EXIT_DEAD";
        case 32:
            return "EXIT_ZOMBIE";
        case 63:
            return "TASK_RUNNING , TASK_INTERRUPTIBLE , TASK_UNINTERRUPTIBLE , __TASK_STOPPED , __TASK_TRACED , EXIT_ZOMBIE , EXIT_DEAD";
        case 64:
            return "TASK_DEAD";
        case 96:
            return "EXIT_ZOMBIE , EXIT_DEAD";
        case 128:
            return "TASK_WAKEKILL";
        case 130:
            return "TASK_WAKEKILL , TASK_UNINTERRUPTIBLE";
        case 132:
            return "TASK_WAKEKILL , __TASK_STOPPED";
        case 136:
            return "TASK_WAKEKILL , __TASK_TRACED";
        case 256:
            return "TASK_WAKING";
        case 512:
            return "TASK_PARKED";
        case 1024:
            return "TASK_NOLOAD";
        case 1026:
            return "TASK_UNINTERRUPTIBLE , TASK_NOLOAD";
        case 2048:
            return "TASK_NEW";
        case 4096:
            return "TASK_STATE_MAX";
        default:
            return "INVALID";
    }
}

/*Register init and exit process*/
module_init(dev_list_init);
module_exit(dev_list_exit);

//information
MODULE_AUTHOR("SADHANA");
MODULE_DESCRIPTION("Character Device to Display Processes");
