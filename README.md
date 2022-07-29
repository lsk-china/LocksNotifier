# LocksNotifier
Send a notification when you press a *Lock key

## Dependencies

- glib
- libnotify

## Build

- Change /dev/input/event4 in main.c to your keyborad device

- Compile with the command below:

  ```shell
  gcc main.c `pkg-config glib-2.0 gdk-pixbuf-2.0 --libs --cflags` -lnotify -o main
  ```

  
