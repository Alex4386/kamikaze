# 🛩️ `kamikaze`
`kamikaze` is a dead-simple `init`, but `SIGKILL`s itself right after initialized.

## What does it do?
Dead simple:  
1. Say hello `"kamikaze started, crashing!"` into kernel ring buffer
2. Immediately `SIGKILL`s itself.

## What is it for?
In kernel development, quickly spin up some kernel and testing is required. usually this is done on QEMU with `serial0` attached for console. Usually this requires horrendous cycle of booting into rootfs and wait until busybox do the init.  

`kamikaze` on init basically:
1. mounts the devtmpfs
2. simply push `"kamikaze started, crashing!"` into kernel message ring.
3. `SIGKILL` itself

So in order to check if your kernel build "boots", you just need to check `serial0` for that specific string and your build is ready to go!

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

## License
This is such a dumb util for doing any sort of licensing.  
Whatever, [UNLICENSE](UNLICENSE)
