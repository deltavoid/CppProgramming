#!/bin/bash
set -xe

# only debian packages
# glibc, libstdc++, boost, some usual packages


apt-get install -y build-essential \
    cmake \
    libboost-dev \
    libgoogle-glog-dev 
    