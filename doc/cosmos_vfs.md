
# CosmOS VFS (Virtual File System)

A core design goal of CosmOS is that it exposes a single root file system that includes all devices and filesystems on all nodes

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
