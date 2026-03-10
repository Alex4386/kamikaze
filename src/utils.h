#ifndef KAMIKAZE_UTILS_H
#define KAMIKAZE_UTILS_H

int ensure_mount();
int get_console_fd();

#ifndef SHOUT_DMESG
#define SHOUT_DMESG(fd, str) do { \
    if ((fd) >= 0) { \
        ssize_t _ret = write((fd), (str), sizeof(str) - 1); \
        (void)_ret; \
        sync(); \
    } \
} while (0)
#endif 

#endif // KAMIKAZE_UTILS_H
