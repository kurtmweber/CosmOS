
# TFS (Trivial File System)

* All blocks are 512 bytes
* Superblock at lba 0
* Map blocks immediately follow Super block
	* one byte per block on the device
* Root dir block follows the mao blocks
	* if it needs to be expanded beyond one data block, data blocks are allocated at that time
* Data blocks follow the Root dir block