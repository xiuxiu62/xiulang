#!/bin/bash

cmake --build build
./build/xc "$@"
