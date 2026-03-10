#ifndef KAMIKAZE_CONFIG_H
#define KAMIKAZE_CONFIG_H

// Set to 1 to allow kamikaze to mount devtmpfs if the kernel didn't.
// Can be overridden at compile time via: gcc -DAUTOMOUNT_DEVTMPFS=0
#ifndef AUTOMOUNT_DEVTMPFS
#define AUTOMOUNT_DEVTMPFS 1
#endif

// The message printed to the kernel ring buffer before crashing.
// The "<1>" prefix sets the log level to KERN_ALERT.
#ifndef KAMIKAZE_MSG
#define KAMIKAZE_MSG "<1>kamikaze started, crashing!\n"
#endif

#endif // KAMIKAZE_CONFIG_H
