![Build](../../workflows/Build/badge.svg)

# What is CosmOS?

CosmOS will be an [SSI](https://en.wikipedia.org/wiki/Single_system_image) operating system. The kernel of CosmOS is 64-bit, protected mode. Our strategy is:

- A single process space spanning multiple physical computers
- A single device space spanning multiple physical computers
- A single file system spanning multiple physical computers
- Automatic process migration between nodes
- Support for ARM and i386

## Screen Shot

![](doc/images/cosmos_screen_shot.png)

## Build and Run CosmOS

- [Building and Running CosmOS](doc/developer/build.md)

## Kernel Documentation

- [Devices](doc/developer/Devices.md)
- [Device Status](doc/developer/device_status.md)
- [Block Devices](doc/developer/block_devices.md)
- [VFS](doc/developer/cosmos_vfs.md)
- [Memory Map](doc/developer/memory_map.md)
- [Collections](doc/developer/Collections.md)
- [Assertions](doc/developer/assertions.md)
- [kprintf](doc/developer/KPrintfConversionSpecifiers.txt)
- [Interrupts](doc/developer/interrupts.md)
- [Console](doc/developer/KernelTerminalConsole.txt)
- [Keyboard](doc/developer/Keyboard.txt)
- [IO Buffers](doc/developer/io_buffers.md)
- [Strings](doc/developer/strings.md)
- File Systems
  - [Trivial File System (TFS)](doc/developer/filesystems/tfs.md)
  - [Cosmos File System (CFS) Disk Format](doc/developer/filesystems/CosmOSDiskFormat.txt)

## Debugging

- [Debugging the kernel](doc/debugging/debug.md)

## Contributing

- [CosmOS coding standards](doc/contributing/CodingStandards.md)
- [Operating System Development Resources](doc/contributing/osdev_resources.md)

## License

- [License](LICENSE)
