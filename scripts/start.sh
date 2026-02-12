#!/bin/bash
# Move to project root
cd "$(dirname "$0")/.."

sudo fuser -vu /dev/snd/* /dev/snd/by-path/*

if [ -f "sbitx" ]; then
    ./sbitx
else
    echo "sbitx binary not found. attempting to build..."
    ./scripts/build
    if [ -f "sbitx" ]; then
        ./sbitx
    fi
fi
bash
