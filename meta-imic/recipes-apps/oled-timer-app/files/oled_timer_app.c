#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OLED_DEVICE  "/dev/oled"
#define TIMER_DEVICE "/dev/my_timer"

#define OLED_TEXT_MAX 96

static volatile sig_atomic_t running = 1;

static void handle_signal(int signal)
{
    (void)signal;
    running = 0;
}

static int write_all(int fd, const char *buffer, size_t length)
{
    size_t offset = 0;

    while (offset < length) {
        ssize_t ret = write(fd, buffer + offset, length - offset);

        if (ret < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }

        offset += (size_t)ret;
    }

    return 0;
}

static int display_frame(int oled_fd, const char *text, uint64_t seconds)
{
    char frame[OLED_TEXT_MAX];
    int length;

    length = snprintf(frame, sizeof(frame),
                      "TEXT: %s\nTIME: %06llu s",
                      text,
                      (unsigned long long)seconds);

    if (length < 0 || (size_t)length >= sizeof(frame)) {
        errno = EOVERFLOW;
        return -1;
    }

    return write_all(oled_fd, frame, (size_t)length);
}

int main(void)
{
    int oled_fd;
    int timer_fd;
    char *text = NULL;
    size_t text_capacity = 0;
    ssize_t text_length;
    struct pollfd timer_pollfd;
    uint64_t counter = 0;

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    printf("Input text: ");
    fflush(stdout);

    text_length = getline(&text, &text_capacity, stdin);
    if (text_length < 0) {
        perror("getline");
        return EXIT_FAILURE;
    }

    if (text_length > 0 && text[text_length - 1] == '\n')
        text[text_length - 1] = '\0';

    oled_fd = open(OLED_DEVICE, O_WRONLY);
    if (oled_fd < 0) {
        perror("open /dev/oled");
        free(text);
        return EXIT_FAILURE;
    }

    timer_fd = open(TIMER_DEVICE, O_RDONLY | O_NONBLOCK);
    if (timer_fd < 0) {
        perror("open /dev/my_timer");
        close(oled_fd);
        free(text);
        return EXIT_FAILURE;
    }

    if (display_frame(oled_fd, text, 0) < 0) {
        perror("write OLED");
        close(timer_fd);
        close(oled_fd);
        free(text);
        return EXIT_FAILURE;
    }

    timer_pollfd.fd = timer_fd;
    timer_pollfd.events = POLLIN;
    timer_pollfd.revents = 0;

    printf("Timer started. Press Ctrl+C to stop.\n");

    while (running) {
        int ret = poll(&timer_pollfd, 1, 2000);

        if (ret < 0) {
            if (errno == EINTR)
                continue;

            perror("poll");
            break;
        }

        if (ret == 0) {
            fprintf(stderr, "Timer timeout: no interrupt received\n");
            continue;
        }

        if (timer_pollfd.revents & (POLLERR | POLLHUP | POLLNVAL)) {
            fprintf(stderr, "Timer device error\n");
            break;
        }

        if (timer_pollfd.revents & POLLIN) {
            ssize_t bytes = read(timer_fd, &counter, sizeof(counter));

            if (bytes < 0) {
                if (errno == EINTR || errno == EAGAIN)
                    continue;

                perror("read timer");
                break;
            }

            if ((size_t)bytes != sizeof(counter)) {
                fprintf(stderr, "Invalid timer data size: %zd\n", bytes);
                break;
            }

            if (display_frame(oled_fd, text, counter) < 0) {
                perror("update OLED");
                break;
            }

            printf("Timer: %llu s\n",
                   (unsigned long long)counter);
        }
    }

    close(timer_fd);
    close(oled_fd);
    free(text);

    return EXIT_SUCCESS;
}