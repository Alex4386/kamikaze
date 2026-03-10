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
    SHOUT_DMESG(fd, KAMIKAZE_ACTUAL_MSG);

    int pid = getpid();
    kill(pid, SIGKILL);

    SHOUT_DMESG(fd, KAMIKAZE_ACTUAL_RAISE_MSG);
    raise(SIGKILL);

    SHOUT_DMESG(fd, KAMIKAZE_ACTUAL_EXIT_MSG);
    exit(1);
    
    return 0;
}
