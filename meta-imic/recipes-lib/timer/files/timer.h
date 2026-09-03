
#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>

// IOCTL command definitions (must match driver)
#define MY_TIMER_IOC_MAGIC    'T'
#define MY_TIMER_START        _IO(MY_TIMER_IOC_MAGIC, 1)
#define MY_TIMER_STOP         _IO(MY_TIMER_IOC_MAGIC, 2)
#define MY_TIMER_GET_COUNT    _IOR(MY_TIMER_IOC_MAGIC, 3, uint32_t)


int open_timer(char* path);

int write_timer(int fd, unsigned int time_value);

int read_timer(int fd);

#endif /* __TIMER_H__ */
