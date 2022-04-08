#!/bin/bash
set -xe

# only debian packages
# glibc, libstdc++, boost, some usual packages


apt-get install -y build-essential \
    pkg-config \
    cmake 
     

apt-get install -y libgoogle-glog-dev \
    libgflags-dev \
    libfmt-dev
    

apt-get install -y libboost-dev 
