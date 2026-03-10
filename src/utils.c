#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include "config.h"

#ifdef __linux__
    #include <sys/mount.h>
#endif

int ensure_devdir() {
    // check if /dev exists
    if (access("/dev", F_OK) != 0) {
        // check if it is "file"
        // if /dev doesn't exist, create it.
        mkdir("/dev", 0755);
    } else {
        // this means, /dev exists.
        // we need to check if it is directory.

        struct stat st;
        stat("/dev", &st);
        if (!S_ISDIR(st.st_mode)) {
            // if it is not directory, we need to remove it and create directory.
            unlink("/dev");
            mkdir("/dev", 0755);
        }
    }
    return 0;
}

int ensure_mount() {
#if defined(__linux__)
    // check kmsg is accessible, else skip
    if (access("/dev/kmsg", F_OK) != 0) {
        if (AUTOMOUNT_DEVTMPFS) {
            // check if rootfs already have screwed up /dev.
            ensure_devdir();

            // Kernel didn't mount it, so we mount it ourselves.
            mount("devtmpfs", "/dev", "devtmpfs", 0, NULL);
        }
    }
#endif
return 0;
}

int get_console_fd() {
#if defined(__linux__)
    return open("/dev/kmsg", O_WRONLY);
#elif defined(__APPLE__)
    return open("/dev/console", O_WRONLY)
#else
    #error "Unsupported platform for kamikaze"
#endif

    // in case of failure, return negative
    return -1;
}
