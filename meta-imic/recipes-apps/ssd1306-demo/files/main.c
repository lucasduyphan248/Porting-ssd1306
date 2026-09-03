#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ssd1306.h"

#define TIMER_DEVICE "/dev/my_timer"
#define TEXT_MAX 80

static volatile sig_atomic_t running = 1;

static void stop_application(int signal_number)
{
    (void)signal_number;
    running = 0;
}

static void display_frame(const char *text, uint64_t seconds)
{
    char time_line[24];

    snprintf(time_line, sizeof(time_line), "TIME: %06llu s",
             (unsigned long long)seconds);

    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString((char *)text, Font_7x10, White);
    ssd1306_SetCursor(0, 16);
    ssd1306_WriteString(time_line, Font_7x10, White);
    ssd1306_UpdateScreen();
}

int main(void)
{
    char *text = NULL;
    size_t capacity = 0;
    ssize_t length;
    int timer_fd;
    struct pollfd timer_pollfd;
    uint64_t counter;

    signal(SIGINT, stop_application);
    signal(SIGTERM, stop_application);

    printf("Input text: ");
    fflush(stdout);
    length = getline(&text, &capacity, stdin);
    if (length < 0) {
        perror("getline");
        return EXIT_FAILURE;
    }

    if (length > 0 && text[length - 1] == '\n')
        text[length - 1] = '\0';
    if (strlen(text) == 0) {
        fprintf(stderr, "Text must not be empty\n");
        free(text);
        return EXIT_FAILURE;
    }
    if (strlen(text) >= TEXT_MAX)
        text[TEXT_MAX - 1] = '\0';

    ssd1306_Init();

    timer_fd = open(TIMER_DEVICE, O_RDONLY | O_NONBLOCK);
    if (timer_fd < 0) {
        perror("open /dev/my_timer");
        free(text);
        return EXIT_FAILURE;
    }

    timer_pollfd.fd = timer_fd;
    timer_pollfd.events = POLLIN;
    timer_pollfd.revents = 0;

    display_frame(text, 0);
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
            fprintf(stderr, "Timer timeout\n");
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
            if (bytes != sizeof(counter)) {
                fprintf(stderr, "Invalid timer data size: %zd\n", bytes);
                break;
            }

            display_frame(text, counter);
            printf("Timer: %llu s\n", (unsigned long long)counter);
        }
    }

    close(timer_fd);
    free(text);
    return EXIT_SUCCESS;
}


