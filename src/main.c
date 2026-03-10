#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <sys/syscall.h>
#include "config.h"
#include "utils.h"

int main() {
    int fd;
    char *msg = KAMIKAZE_MSG;

    ensure_mount();

    // kernel msg ring go brrr
    fd = get_console_fd();
    if (fd >= 0) {
        write(fd, msg, sizeof(KAMIKAZE_MSG) - 1);
        close(fd);
    } else {
        // dev mount epic fail
        printf("kamikaze: /dev/kmsg unavailable. Crashing anyway!\n");
    }

    sync();

    int pid = getpid();
    kill(pid, SIGKILL);
    
    const char* syscall_note = "kamikaze: SIGKILL was not working properly. trying raising SIGKILL";
    fd = get_console_fd();
    if (fd >= 0) {
        write(fd, syscall_note, sizeof(syscall_note) - 1);
        close(fd);
        sync();
    }

    raise(SIGKILL);
    
    const char* spinlock_note = "kamikaze: syscall fallback failed. failing over to exiting";
    fd = get_console_fd();
    if (fd >= 0) {
        write(fd, spinlock_note, sizeof(spinlock_note) - 1);
        close(fd);
        sync();
    }

    exit(1);
    
    return 0;
}
