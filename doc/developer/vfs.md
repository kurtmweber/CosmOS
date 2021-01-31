
# CosmOS Virtual File System

CosmOS has a hierarchical file system, similar to POSIX. A VFS node is defined as:

```java
struct vfs {
    enum vfs_type type;
    struct arraylist* children;

    /*
    This is a filename or a folder name
    For a device, or a fs, it's the dev name, since a fs is a dev
    */
    uint8_t* name;

    vfs_open_function open;
    vfs_read_function read;
    vfs_write_function write;
    vfs_close_function close;
    vfs_close_readdir_function readdir;
};
```

CosmoS VFS nodes can be files, folders, or devices.  CosmOS OS uses devices for file systems, and therefore does not need mount points represented in vfs's.

## Design


# CosmOS VFS (Virtual File System)

A core design goal of CosmOS is that it exposes a single root file system that includes all devices and filesystems on all nodes

## VFS Hierarchy

The basic hierarchy seen by every node is

```
	<node_name>/<filesystem_name>
```

and

```
	<node_name>/dev/<device_name>
```

For example presuming we have two nodes "bart" and "lisa"

```
/
/bart/fat0
/bart/dev/serial0
/lisa/fat0
/lisa/fat1
/lisa/dev/serial0
/lisa/dev/serial1
```

## Sync

The CosmOS cluster will sync the VFS implementations on each node as nodes come online and go offline, and as devices are attached and detached

## Nodes

CosmOS will expose a `node` filesystem which contains information for each node in the system such as 

* Uptime
* Total RAM in the system
* Total RAM in use
* Total Processors in the system
* Processor Usage
* Network throughput

This information will be useful for process migrations.

For example

```
/node/bart
/node/lisa

```

## Processes

In theory, CosmOS can also expose a `process` filesystem, similar to the `dev` filesystem which contains every process on every node.  For example

```
/bart/process/process_abc
/bart/process/process_def
```
