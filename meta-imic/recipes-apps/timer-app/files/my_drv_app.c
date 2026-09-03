#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "my_drv.h"

int main ()
{
    int fd;
    char output[BUFFER_SIZE];
    const char *input = "hello wolrd";
    ssize_t bytes;

    //open the device
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0){
        perror("Failled to open device");
        return EXIT_FAILURE;
    }

    // Write fixed string to the driver

    // - fd: file descriptor trả về từ open(), đại diện cho thiết bị/driver.
    // - input: con trỏ tới buffer chứa dữ liệu cần ghi.
    // - strlen(input): số byte cần ghi từ buffer input.
    // Hàm trả về số byte đã ghi thành công, hoặc -1 nếu có lỗi.
    bytes = write(fd, input, strlen(input));
    if (bytes < 0){
        perror("Failed to write");
        close(fd);
        return EXIT_FAILURE;
    }

    //Apply transformations through ioctl
    if (ioctl(fd, IOCTL_STRING_CAPITALIZE) < 0){
        perror("IOCTL capitalize failed");
        close(fd);
        return EXIT_FAILURE;
    }

    if (ioctl(fd, IOCTL_STRING_REVERSE) < 0){
        perror("IOCTL capitalize failed");
        close(fd);
        return EXIT_FAILURE;
    }

    //REad transformed string back
    bytes = read(fd, output, sizeof(output) - 1);
    if (bytes < 0){
        perror("Read failed");
        close(fd);
        return EXIT_FAILURE;
    }

    output[bytes] = '\0';
    printf("Original string: %s\n", input);
    printf("Transformed string: %s\n", output);

    close(fd);
    return EXIT_SUCCESS;
}

