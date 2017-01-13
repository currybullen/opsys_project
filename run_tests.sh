#!/bin/bash
#
# Simple bash script that will load the kernel module, run a suite of tests and
# unload it

# Load the kernel module
insmod out/kvs.ko

# Run the tests
./out/kvs.o /dev/kvs

# Remove the kernel module
rmmod kvs
