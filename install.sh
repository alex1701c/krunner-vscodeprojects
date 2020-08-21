#!/bin/bash

# Exit if something fails
set -e

if [[ $(basename "$PWD") != "krunner-vscodeprojects"* ]];then
    git clone https://github.com/alex1701c/krunner-vscodeprojects
    cd krunner-vscodeprojects/
fi

mkdir -p build
cd build

cmake -DCMAKE_INSTALL_PREFIX=/usr -DKDE_INSTALL_QTPLUGINDIR=`kf5-config --qt-plugins` -DCMAKE_BUILD_TYPE=Release  ..
make -j$(nproc)

sudo make install

kquitapp5 krunner
kstart5 --windowclass krunner krunner

echo "Installation finished !";
