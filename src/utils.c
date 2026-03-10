#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include "config.h"

#ifdef __linux__
    #include <sys/mount.h>
#endif


int ensure_mount() {
#if defined(__linux__)
    // check kmsg is accessible, else skip
    if (access("/dev/kmsg", F_OK) != 0) {
        if (AUTOMOUNT_DEVTMPFS) {
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
