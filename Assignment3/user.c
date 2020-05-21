//
//  user.c
//  Assignment3
//
//  Created by Sadhana Desai on 4/28/20.
//  Copyright © 2020 Sadhana Desai. All rights reserved.
//
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<assert.h>
#include<string.h>
//#include "Header.h"

#define SIZE 500
struct Buffer
{
    unsigned long address;
    long time;
};
int main()
{
    struct Buffer buf[SIZE];
    int i=0;
    int errorFlag = 0;
    int fp = open("/proc/Process_info",O_RDONLY);
    errorFlag = read(fp,buf,sizeof(buf));
        //printf("Page Fault Address           Time\n");
    for(i=0;i<SIZE;i++)
    {
        printf("0x%lx, %ld\n",buf[i].address,buf[i].time);
    }
    close(fp);
    return 0;
}
