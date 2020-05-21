//
//  main.c
//  Assignment3
//
//  Created by Sadhana Desai on 4/25/20.
//  Copyright © 2020 Sadhana Desai. All rights reserved.
//

#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/kprobes.h>
#include<linux/time.h>
#include<linux/limits.h>
#include<linux/sched.h>
#include<linux/fs.h>
#include<linux/memory.h>
#include<linux/memcontrol.h>
#include<linux/proc_fs.h>
#include<asm/uaccess.h>
#include<linux/slab.h>
#include<linux/uaccess.h>

#define BUFFER_SIZE 1000
#define MAX_SYMBOL_LEN 64

pid_t pr_id;
module_param(pr_id,int,0);
#define PROCESS_NAME "Process_info"

static char symbol[MAX_SYMBOL_LEN] = "handle_mm_fault";
module_param_string(symbol,symbol,sizeof(symbol),0644);

struct DataBuffer
{
    unsigned long address;
    long time;
};
struct DataBuffer buffer[BUFFER_SIZE];


static ssize_t read_process(struct file *file, char *out,size_t count,loff_t *offp);

/* a kprobe structure */
static struct kprobe kp= {
    .symbol_name    = symbol,
};

static ssize_t read_process(struct file *file,char *out,size_t count,loff_t *offp)
{
    int ret=0,i=0;
    printk("Read:\n");

    for(i=0;i<BUFFER_SIZE;i++)
    {
        ret=copy_to_user(&out[i],&buffer[i],sizeof(buffer[i]));

        if(ret == 0)

        if(ret<0)
        {
            printk(KERN_ALERT "Copy!");
            ret = -1;
        }
    }
    return ret;
}

//** File operations **//

static struct file_operations process_fops = {
    .read= &read_process,
    //.unlocked_ioctl = &device_ioctl,
};

/* kprobe handler:*/
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
 struct timespec pr_time;
 int k = 0;
 #ifdef CONFIG_X86
   if(current->pid == pr_id)
   {
       pr_time = current_kernel_time();
       buffer[k].address = regs->si;
       buffer[k].time = pr_time.tv_nsec;
       printk(KERN_INFO "pid = %ld, virtual address = %lx, time = %ld \n",current->pid, buffer[k].address,buffer[k].time);
       k++;
   }
 #endif
    return 0;
}

static int __init kprobe_init(void)
{
    int ret;
    kp.pre_handler = handler_pre;
    struct proc_dir_entry *proc_file_entry;
    ret = register_kprobe(&kp);
    if (ret < 0) {
        pr_err("register_kprobe failed, returned %d\n", ret);
        return ret;
    }
    pr_info("Planted kprobe at %p\n", kp.addr);
    proc_file_entry =proc_create(PROCESS_NAME,0,NULL,&process_fops);
    if(proc_file_entry==NULL)
    {
        printk(KERN_INFO "Proc failed to enter!\n");
          return -1;
    }
    return 0;
}

static void __exit kprobe_exit(void)
{
    unregister_kprobe(&kp);
    remove_proc_entry(PROCESS_NAME,NULL);
    pr_info("kprobe at %p unregistered\n", kp.addr);
}

module_init(kprobe_init);
module_exit(kprobe_exit);
MODULE_LICENSE("GPL");
