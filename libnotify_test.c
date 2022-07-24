#include <libnotify/notify.h>
#include <libnotify/notification.h>
#include <stdio.h>
#include <glib.h>

int main () {
    notify_init("Test notify");
    NotifyNotification *not;
    not = notify_notification_new("Test notification!", NULL, NULL);
    GError *err;
    notify_notification_show(not, &err);
    return 114514;
}
