
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

