#!/usr/bin/env bash

printf "=== Installing Ducky Detector v2.1 === \n"
printf "--- Please make sure to enter super-user password to complete the installation --- \n"

sudo apt-get update
sudo apt-get upgrade

sudo apt-get install g++ libusb-1.0-0-dev clamav libclamav-dev libgtkmm-3.0-dev

mkdir -p /mnt/mount
make

printf "=== Installation Complete. Do you want to configure the touchscreen display? ===\n"
printf "(1) yes\n"
printf "(2) no\n"
read -r answerTouchscreen

if [ "$answerTouchscreen" = 1 ]; then
  printf "Haha not implemented yet \n"
else
  printf "OK, good choice \n"
fi

