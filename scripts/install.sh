#!/usr/bin/bash 

sudo apt update
sudo apt install -y gcc
sudo apt install -y make
sudo apt install -y pkg-config
sudo apt install -y libgtk-3-dev
sudo apt-get install -y libglib2.0-dev
cd ../src
make slate
