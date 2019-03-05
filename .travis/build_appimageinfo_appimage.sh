#!/usr/bin/env bash

set -xe

# Build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release  .
make -j$(nproc)

# Create AppDir
DESTDIR=AppDir make install

# Build AppImage
wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
chmod +x linuxdeploy*-x86_64.AppImage
#find AppDir -type f -iname '*.so*' -print0 | xargs -0 -i -n1 sh -c 'echo {}; strip {}' \;

export NO_STRIP=1
./linuxdeploy-x86_64.AppImage --appdir=AppDir --output appimage
