#ifndef KAMIKAZE_CONFIG_H
#define KAMIKAZE_CONFIG_H

// Set to 1 to allow kamikaze to mount devtmpfs if the kernel didn't.
// Can be overridden at compile time via: gcc -DAUTOMOUNT_DEVTMPFS=0
#ifndef AUTOMOUNT_DEVTMPFS
#define AUTOMOUNT_DEVTMPFS 1
#endif

// The message printed to the kernel ring buffer before crashing.
// The "<1>" prefix sets the log level to KERN_ALERT.
#ifndef KAMIKAZE_LOG_LEVEL
#define KAMIKAZE_LOG_LEVEL "<1>"
#endif

#ifndef KAMIKAZE_MSG
#define KAMIKAZE_MSG "kamikaze: kamikaze started, crashing!"
#endif

#ifndef KAMIKAZE_RAISE_MSG
#define KAMIKAZE_RAISE_MSG "kamikaze: SIGKILL was not working properly. trying raising SIGKILL"
#endif

#ifndef KAMIKAZE_EXIT_MSG
#define KAMIKAZE_EXIT_MSG "kamikaze: raising SIGKILL fallback failed. failing over to exiting"
#endif

// == actual msg ==
#define KAMIKAZE_ACTUAL_MSG KAMIKAZE_LOG_LEVEL KAMIKAZE_MSG "\n"
#define KAMIKAZE_ACTUAL_RAISE_MSG KAMIKAZE_LOG_LEVEL KAMIKAZE_RAISE_MSG "\n"
#define KAMIKAZE_ACTUAL_EXIT_MSG KAMIKAZE_LOG_LEVEL KAMIKAZE_EXIT_MSG "\n"

#endif // KAMIKAZE_CONFIG_H
