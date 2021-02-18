#!/bin/sh
set -e
. ./config.sh

# shellcheck disable=SC2153
for PROJECT in $PROJECTS; do
  (cd "$PROJECT" && $MAKE clean)
done

rm -rf sysroot
rm -rf isodir
rm -rf paios.iso
