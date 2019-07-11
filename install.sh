#!/bin/bash

if [[ $(basename "$PWD") != "VSCodeProjectsRunner"* ]];then
    git clone https://github.com/alex1701c/VSCodeProjectsRunner
    cd VSCodeProjectsRunner/
fi

mkdir -p build
cd build
cmake -DQT_PLUGIN_INSTALL_DIR=`kf5-config --qt-plugins` ..
make -j2
sudo make install
kquitapp5 krunner 2> /dev/null
kstart5 --windowclass krunner krunner > /dev/null 2>&1 &

echo "Installation finished !";
