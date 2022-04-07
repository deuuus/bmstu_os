#!/bin/bash
make
echo ">>>>> Loading md"
insmod md.ko
lsmod | grep md
echo ">>>>> System log after module insert:"
dmesg | tail -n40| grep Process
dmesg | tail -n40| grep Module
echo ">>>>> Removing md"
rmmod md
echo ">>>>> System log after module removal:"
dmesg | tail -n40| grep Module
dmesg | tail -n5| grep Process