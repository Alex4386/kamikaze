# 🛩️ `kamikaze`
`kamikaze` is a dead-simple `init`, but `SIGKILL`s itself right after initialized. ("ragequit" included)

<center>
  <img width="1513" height="831" alt="image" src="https://github.com/user-attachments/assets/e7d86f82-6979-44ea-9d97-044071e4e82e" />
</center>

## What does it do?
Dead simple:  
1. Say hello `"kamikaze started, crashing!"` into kernel ring buffer
2. Immediately `SIGKILL`s itself.

## What is it for?
In kernel development, quickly spin up some kernel and testing is required. usually this is done on QEMU with `serial0` attached for console. Usually this requires horrendous cycle of booting into rootfs and wait until busybox do the init.  
**This is where `kamikaze` comes in.** It checks whether kernel is OK state to boot into userland, and then it kills itself.  
  
`kamikaze` on init basically:
1. mounts the devtmpfs
2. simply push `"kamikaze started, crashing!"` into kernel message ring.
3. `SIGKILL` itself.
    1. If this fails, it fallbacks to `raise`.
    2. If this also fails, it just `exit(1)`.

So in order to check if your kernel build "boots", you just need to check `serial0` for that specific string and your build is ready to go!

## FAQ
1. Can't you just call `panic()` instead of `SIGKILL`ing yourself?  
   > Dude, this is `init`! A userland process, not a kernel module. `panic()` is not available here.
2. Why do you even need to mount `devtmpfs`?
   > Just in case you screwed up your rootfs. You can disable this behavior by defining `-DAUTOMOUNT_DEVTMPFS=0` when building.
3. Why not do ACPI Shutdown instead?
    > 1. `ACPI` is Microsoft stuff, So you shouldn't expect all platforms to implement it.  
    > 2. Using `SIGKILL` allows `kamikaze` to be used in `LXC` environments, allowing `kamikaze`'d LXC (which would serve no purpose but still)  
    > 3. Causing kernel panic to kill kernel is faster than graceful system shutdown  
    >    (as a added bonus, you get a nice stack trace in the kernel log)
4. Why this name?
    > First, it's [kills itself](https://en.wikipedia.org/wiki/Kamikaze)  
    > Second, this is sorta-ragebait-name for regional developers, ya know? easy to remember, lol.

## Usage
Build just the statically linked binary:
```bash
make
```

Or build binary and pack it into initramfs archive:
```
make cpio
```

> [!NOTE]  
> You can test built initramfs with (suppose you are testing kernel image `bzImage`):  
> ```bash
> qemu-system-x86_64 -kernel bzImage -initrd kamikaze.cpio -nographic -append "console=ttyS0"
> ```

## Integration
See [ci/run-qemu.sh](ci/run-qemu.sh) for an example. Basically you need to do the following steps:
1. Build `rootfs` (You can use `make cpio`)
2. Bring your built kernel image
3. Run QEMU with `-kernel` flag pointing to your kernel and `console=ttyS0` so `dmesg` can be read from `serial0`.
4. save that into file, and check for `"kamikaze started, crashing!"` string with grep or something (If you have defined custom string with `-DKAMIKAZE_MSG=your_message`, check for that instead)

## License
This is such a dumb util for doing any sort of licensing.  
Whatever, [UNLICENSE](UNLICENSE)
