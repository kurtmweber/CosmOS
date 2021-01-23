
# qemu
QEMU=qemu-system-x86_64
QEMUARGS=                                                 \
  -accel tcg,thread=single                                \
  -cpu core2duo                                           \
  -m 4096                                                \
  -no-reboot                                              \
  -drive format=raw,file=hda.img                          \
  -smp 1                                                  \
  -netdev user,id=n0,hostfwd=tcp::8080-:80  \
  -device rtl8139,netdev=n0       \
  -object filter-dump,id=f1,netdev=n0,file=dump.dat      \
  -serial stdio                                           \
  -audiodev coreaudio,id=audio0                           \
  -device adlib,audiodev=audio0                          \
  -monitor telnet::45454,server,nowait                    \
  -drive file=mbr_fat.img,index=1,format=raw           \
  -drive file=blank.img,index=2,format=raw          \
  -D qemu.log                  

 #  -device usb-ehci                                        \

#  -drive if=virtio,file=hda.img,format=raw                \
#  -device sb16,audiodev=audio0                            \
# -nic user,model=virtio-net-pci                          \
 
#   -nic user,model=rtl8139,net=192.168.76.0/24,dhcpstart=192.168.76.9,id=net1 \


# note that we're mounting the hda.img as a floppy image. good enough for now.
# the first bytes are FA B4 00 B0 03 CD 10 88 16 7C 7C B8 01 24 CD 15

# to list devices:
# qemu-system-x86_64 -device ?

# -device AC97,audiodev=audio0
# -device adlib,audiodev=audio0

#  -machine pcspk-audiodev=audio0                          \

# -device floppy,drive=f0                                 \
#  -blockdev driver=file,node-name=f0,filename=hda.img     \
 
# -device sdhci-pci                                       \
#  -blockdev driver=file,node-name=sddrive,filename=hda.img \
#  -device sd-card                                                   
