#!/bin/bash
set -xe


(
    cd cpp/example/hello_cmake
    make build 
    make run
)
