#!/bin/bash
#/* **************** LFD5420:4.13 s_21/do_kexec.sh **************** */
#/*
# * The code herein is: Copyright the Linux Foundation, 2017
# *
# * This Copyright is retained for the purpose of protecting free
# * redistribution of source.
# *
# *     URL:    http://training.linuxfoundation.org
# *     email:  trainingquestions@linuxfoundation.org
# *
# * This code is distributed under Version 2 of the GNU General Public
# * License, which you should have received with the source.
# *
# */
#!/bin/bash

KV="$1" && [ -z "$KV" ] && KV=$(uname -r)
KERNEL=/boot/vmlinuz-$KV
CMDLINE=$(cat /proc/cmdline)

# Various names for initramfs/initrd depending on distro :)
INITRD=/boot/initramfs-"$KV".img
[ ! -f "$INITRD" ] && INITRD=/boot/initramfs.img-"$KV"
[ ! -f "$INITRD" ] && INITRD=/boot/initrd-"$KV".img
[ ! -f "$INITRD" ] && INITRD=/boot/initrd.img-"$KV"
[ ! -f "$INITRD" ] && echo Kernel version $KV not installed && exit

echo kexec -l --initrd=$INITRD --command-line="$CMDLINE"  $KERNEL
     kexec -l --initrd=$INITRD --command-line="$CMDLINE"  $KERNEL
