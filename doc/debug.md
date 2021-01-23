
# Debugging

You can attach Gdb to the running Qemu guest for kernel debugging. Use `make qemu-debug` to make QEMU listen for an incoming connection from gdb on TCP port 1234, and not start the guest until you tell it to from gdb.

```bash
$ make qemu-debug
```

Just attaching the gdb to the guest VM doesn’t get you anywhere. You need to set a breakpoint at the code you want to examine and trigger the execution of that code in the guest VM.

In another terminal, start Gdb:
```bash
$ gdb kernel/cosmos.elf
```

Connect to the remote terminal:
```GDB
(gdb) target remote :1234
Remote debugging using :1234
0x000000000000fff0 in ?? ()
```

Set a breakpoint, step into functions, and print a backtrace:
```GDB
(gdb) break cosmos.c:CosmOS
Breakpoint 1 at 0xffff800000000000: file cosmos.c, line 35.
(gdb) c
Continuing.

Breakpoint 1, CosmOS () at cosmos.c:35
35	void CosmOS() {
(gdb) s
44	    kprintf("Loading CosmOS 0.1\n");
(gdb) s
kprintf (s=0x0) at sys/kprintf/kprintf.c:16
16	uint64_t kprintf(const char *s, ...) {
(gdb) bt
#0  kprintf (s=0x0) at sys/kprintf/kprintf.c:16
#1  0xffff800000000019 in CosmOS () at cosmos.c:44
Backtrace stopped: previous frame inner to this frame (corrupt stack?)
(gdb) 
```

## VSCode debugging
If you prefer VSCode, configure `launch.json` like so:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "type": "gdb",
            "request": "attach",
            "name": "Attach to gdbserver",
            "executable": "kernel/cosmos.elf",
            "target": ":1234",
            "remote": true,
            "cwd": "${workspaceRoot}",
            "valuesFormatting": "parseText"
        }
    ]
}
```