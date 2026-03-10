#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
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
    kill(getpid(), SIGKILL);

    // we shouldn't reach here at the first point,
    // spinlock the heck out of it
    while(1) {
        sleep(1);
    }
    
    return 0;
}