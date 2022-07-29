#include <libnotify/notify.h>
#include <libnotify/notification.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/input.h>
#include <glib.h>
#include <fcntl.h>
#include <signal.h>

int fd, stop = 0;
GError *err;

void sigint() {
    close(fd);
    notify_uninit();
    stop = 1;
}

void send_notification(char *message) {
    NotifyNotification *not = notify_notification_new(message, NULL, NULL);
    notify_notification_show(not, NULL);
}

int main() {
    pid_t pid = fork();
    if (pid > 0) {
        printf("Successfully forked process, child process PID: %d\n", pid);
        return 0;
    } else if (pid == 0) {
        return child_main();
    } else {
        printf("Fork process failed\n");
        return 1;
    }
}

int child_main() {
    signal(SIGINT, sigint);
    int ret = 0;
    struct input_event ev;

    fd = open("/dev/input/event4", O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Cannot open event device\n");
        return 1;
    }
    
    if(!notify_init("Lock Key Notifiy")) {
        fprintf(stderr, "Cannot init libnotify\n");
        close(fd);
        return 1;
    }

    do {
        memset(&ev, 0, sizeof(struct input_event));
        ret = read(fd, &ev, sizeof(struct input_event));
        if (!ret) {
            fprintf(stderr, "Cannot read from event device\n");
            sigint();
            return 1;
        }
        if (ev.type == EV_KEY && ev.value == 0) {
            switch(ev.code) {
                case KEY_CAPSLOCK:
                    send_notification("You pressed CAPSLOCK");
                    break;
                case KEY_NUMLOCK:
                    send_notification("You pressed NUMLOCK");
                    break;
                case KEY_SCROLLLOCK:
                    send_notification("You pressed SCROLL_LOCK");
                    break;
                default:
                    continue;
            }
            if (err) {
                fprintf(stderr, "Send notification failed: %s\n", err->message);
                g_error_free(err);
                sigint();
                return 1;
            }
        }
    } while (!stop);
    return 0;
}
