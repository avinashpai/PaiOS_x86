#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/paios.kernel isodir/boot/paios.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "paios" {
	multiboot /boot/paios.kernel
}
EOF
grub-mkrescue -o paios.iso isodir
