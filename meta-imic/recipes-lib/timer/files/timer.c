#include "timer.h"

int open_timer(char* path){
    int fd = open(path, O_RDWR);
    if (fd < 0) {
        printf("Failed to open %s\n",path);
        return EXIT_FAILURE;
    }

    return fd;
}

int write_timer(int fd, unsigned int time_value) {
    
    //write timer value to timer register
    //use write or IOCTL
    return 0;
}

int read_timer(int fd) {
    //read current timer counter
    return 0;
}
