#ifndef MY_DRV_H
#define MY_DRV_H

#ifdef __KERNEL__
#include <linux/ioctl.h>
#else
#include <sys/ioctl.h>
#endif

#define DRIVER_NAME "my_drv"
#define CLASS_NAME  "my_class"
#define DEVICE_PATH "/dev/my_drv"

#define IOCTL_MAGIC 'S'
#define IOCTL_STRING_REVERSE    _IO(IOCTL_MAGIC, 1)
#define IOCTL_STRING_CAPITALIZE _IO(IOCTL_MAGIC, 2)
#define BUFFER_SIZE 256

#endif /* MY_DRV_H */
