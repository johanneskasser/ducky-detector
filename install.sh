#!/bin/bash

user=$(whoami)

if [ "$user" != "root" ]; then
  echo "=== FATAL ERROR: You need to execute this script as root user! ==="
  exit
else
  echo "=== Installing Ducky Detector v2.1 ==="
  echo "--- Installing required packages... (This may take a while) ---"

  loading 1
  sudo apt-get update &>> ./debug.txt
  sudo apt-get upgrade &>> ./debug.txt

  sudo apt-get --assume-yes install g++ libusb-1.0-0-dev clamav libclamav-dev libgtkmm-3.0-dev &>> ./debug.txt

  mkdir -p /mnt/mount
  sudo make

  loading 0

  echo "=== Installation Complete. Do you want to configure the touchscreen display? ==="
  echo "(1) yes"
  echo "(2) no"
  read -rp "> " answerTouchscreen

  if [ "$answerTouchscreen" = 1 ]; then
    loading 1
    echo "Haha not implemented yet"
    loading 0
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

function loading() {
  printf "Loading, please wait.. "

  sp="/-\|"
  sc=0

  working=$1


  until [ "$working" -lt 1 ]
  do
     spin
  done
  endspin
}

sp="/-\|"
sc=0

function spin() {

   printf "\b${sp:sc++:1}"
   ((sc==${#sp})) && sc=0
  }

# shellcheck disable=SC2120
function endspin() {
    printf "\r%s\n" "$@"
}
