#!/bin/bash

set -eu

IMAGE="disk.qcow2"
IMAGE2="disk.raw"
MEMORY="2G"
CPU="2"
VNC="10003"
ISO="FILE.iso"

qemu-system-x86_64 \
	-m "$MEMORY" \ # RAM
	-smp  "$CPU" \ # Number of CPUs - can configure; sockets,cpus,cores,threads
	-accel kvm \ # hardware accelerated using kvm
	-cdrom $ISO \ # ISO image
	-boot d \ # Boot order; first CD-ROM
	-drive file="$IMAGE",format=qcow2,if=virtio \ # COW disk with virtio
	-drive file="$IMAGE2",format=raw,if=virtio \ # RAW disk with virtio
	-vnc "127.0.0.1:$((VNC - 5900))" \ # VNC
	-netdev user,id=net0,hostfwd=tcp::5555-:22 \ # virtual network device with an ID and Host forward set for sshd.service
	-device e1000,netdev=net0 # The type of device (e1000)
