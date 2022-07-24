#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int fd = -1, stop = 0;

void handle_sigint() {
    printf("stop now...\n");
    stop = 1;
}

int main() {
    sigset(SIGINT, handle_sigint);
    int ret = -1;
    struct input_event event;
    fd = open("/dev/input/event4", O_RDONLY);

    do {
        memset(&event, 0, sizeof(struct input_event));
        ret = read(fd, &event, sizeof(struct input_event));
	    printf("event.type: %X, event.code: %d, event.value: %X\n", event.type, event.code, event.value);
        if (event.type == EV_KEY && event.value == 0) {
            switch(event.code) {
                case KEY_CAPSLOCK:
                    printf("You pressed capslock\n");
                    break;
                case KEY_NUMLOCK:
                    printf("You pressed numlock\n");
                    break;
                case KEY_SCROLLLOCK:
                    printf("You pressed scrolllock\n");
                    break;
            }
        }
    } while (!stop);
    close(fd);
}
