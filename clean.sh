#!/bin/sh

echo "cleaning all generated files"
find . -type f -name '*.o' -exec rm {} +
find . -type f -name '*.a' -exec rm {} +
find . -type f -name '*.bin' -exec rm {} +
find . -type f -name '*.elf' -exec rm {} +