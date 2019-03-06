#!/usr/bin/env bash

set -xe

pushd /tmp

git clone https://github.com/AppImage/libappimage.git

pushd libappimage
git submodule update --init --recursive

cmake . -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX=/usr -DUSE_SYSTEM_BOOST=On \
        -DUSE_SYSTEM_LIBARCHIVE=On -DUSE_SYSTEM_XZ=On

sudo make install

popd
popd
