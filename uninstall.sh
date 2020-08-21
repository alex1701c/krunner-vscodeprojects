#!/bin/bash

# Exit if something fails
set -e

cd build
sudo make uninstall

# KRunner needs to be restarted for the changes to be applied
kquitapp5 krunner
kstart5 --windowclass krunner krunner
