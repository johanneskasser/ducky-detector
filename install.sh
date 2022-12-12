#!/bin/bash

user=$(whoami)

if [ "$user" != "root" ]; then
  echo "=== FATAL ERROR: You need to execute this script as root user! ==="
  exit
else
  echo "=== Installing Ducky Detector v2.1 ==="

  sudo apt-get update
  sudo apt-get upgrade

  sudo apt-get install g++ libusb-1.0-0-dev clamav libclamav-dev libgtkmm-3.0-dev

  mkdir -p /mnt/mount
  sudo make

  echo "=== Installation Complete. Do you want to configure the touchscreen display? ==="
  echo "(1) yes"
  echo "(2) no"
  read -rp "> " answerTouchscreen

  if [ "$answerTouchscreen" = 1 ]; then
    echo "Haha not implemented yet"
  else
    echo "OK, good choice"
  fi

  echo "=== Do you want to start the ducky-detector everytime the raspberry pi is started? ==="
  echo "(1) yes"
  echo "(2) no"
  read -rp "> " answerAutoStart

  if [ "$answerAutoStart" = 1 ]; then
    echo "Not Implemented yet"
  else
    echo "=== Installation Complete. Starting ducky-detector! ==="
    sudo ./DuckyDetector
  fi
fi

