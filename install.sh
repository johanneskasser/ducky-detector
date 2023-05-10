#!/bin/bash

width=40
steps=8
completedSteps=0

prog() {
  completedSteps=$((completedSteps + 1))
  percentage=$(bc <<<"scale=2; $completedSteps / $steps")
  percentageToPrint=$(bc <<<"scale=2; $percentage * 100")
  stepsToPrint=$(printf "%.0f" "$(bc <<<"scale=2; $width * $percentage")")
  remainingSteps=$((width - stepsToPrint))

  echo -ne '['$(for i in $(seq 1 $stepsToPrint); do printf "#"; done) $(for i in $(seq 1 $remainingSteps); do printf '_'; done) '] ('$(echo $percentageToPrint)'%)\r'

  if [ $completedSteps -eq $steps ]; then
    echo -ne '\n'
  fi
}

function installTouchScreen {
  echo "=== Configuration of Touchscreen ==="

  if [ -f "/boot/config.txt" ]; then
    echo "#Joy-IT Touchscreen configuration"
    echo "dtparam=spi=on" >>/boot/config.txt
    echo "dtoverlay=joy-IT-Display-Driver-32b-overlay:rotate=270,swapxy=1" >>/boot/config.txt
    echo "dtparam=audio=on" >>/boot/config.txt
    echo "max_usb_current=1" >>/boot/config.txt
    echo "hdmi_force_hotplug=1" >>/boot/config.txt
    echo "config_hdmi_boost=7" >>/boot/config.txt
    echo "hdmi_drive=1" >>/boot/config.txt
    echo "hdmi_ignore_edid=0xa5000080" >>/boot/config.txt

    sed /dtoverlay=vc4-fkms-v3d/d /boot/config.txt
  else
    echo "ERROR: OS does not seem to be Raspberry OS! -> File /boot/config.txt cannot be found!"
    return
  fi

  if [ -f "/boot/cmdline.txt" ]; then
    echo " fbcon=map:10"
  else
    echo "ERROR: OS does not seem to be Raspberry OS! -> File /boot/cmdline.txt cannot be found!"
    return
  fi

  echo "Section "InputClass"" >>/usr/share/X11/xorg.conf.d/99-calibration.conf
  echo "Identifier "calibration"" >>/usr/share/X11/xorg.conf.d/99-calibration.conf
  echo "MatchProduct "ADS7846 Touchscreen"" >>/usr/share/X11/xorg.conf.d/99-calibration.conf
  echo "Option "Calibration" "160 3723 3896 181"" >>/usr/share/X11/xorg.conf.d/99-calibration.conf
  echo "Option "SwapAxes" "0"" >>/usr/share/X11/xorg.conf.d/99-calibration.conf
  echo "EndSection" >>/usr/share/X11/xorg.conf.d/99-calibration.conf

}

function enableAutoStartup {
  echo "=== Configuration of Autostart ==="

  mkdir /home/pi/.config/autostart

  echo "[Desktop Entry]" >>/home/pi/.config/autostart/duckyDetector.desktop
  echo "Name=DuckyDetector" >>/home/pi/.config/autostart/duckyDetector.desktop
  echo "Exec=sudo /home/pi/ducky-detector/DuckyDetector" >>/home/pi/.config/autostart/duckyDetector.desktop
  echo "Type=Application" >>/home/pi/.config/autostart/duckyDetector.desktop

  if [ -f "/home/pi/.config/autostart/duckyDetector.desktop" ]; then
    echo "Startup configuration successful!"
  else
    echo "Something went wrong"
  fi
}

user=$(whoami)

update_bar 0

if [ "$user" != "root" ]; then
  echo "=== FATAL ERROR: You need to execute this script as root user! ==="
  exit
else
  echo "=== Installing Ducky Detector v2.1 ==="
  echo "--- Installing required packages (This may take a while)... ---"

  sudo apt-get update &>>./debug.txt
  prog
  sudo apt-get upgrade &>>./debug.txt
  prog

  sudo apt-get --assume-yes install g++ &>>./debug.txt
  prog
  sudo apt-get --assume-yes install clamav &>>./debug.txt
  prog
  sudo apt-get --assume-yes install libclamav-dev &>>./debug.txt
  prog
  sudo apt-get --assume-yes install llibgtkmm-3.0-dev &>>./debug.txt
  prog

  mkdir -p /mnt/mount
  prog
  sudo make &>>./debug.txt
  prog

  echo "=== Installation Complete. Do you want to configure the touchscreen display? ==="
  echo "(1) yes"
  echo "(2) no"
  read -rp "> " answerTouchscreen

  if [ "$answerTouchscreen" = 1 ]; then
    installTouchScreen
  else
    echo "Proceeding to automatic Start"
  fi

  echo "=== Do you want to start the ducky-detector everytime the raspberry pi is started? ==="
  echo "(1) yes"
  echo "(2) no"
  read -rp "> " answerAutoStart

  if [ "$answerAutoStart" = 1 ]; then
    enableAutoStartup
  fi
  if [ -f "./DuckyDetector" ]; then
    echo "=== Installation Complete. Starting ducky-detector! ==="
    rm ./debug.txt
    sudo ./DuckyDetector
  else
    echo "=== Something went wrong! Please Consult the debug.txt file for further information! ==="
  fi
fi
