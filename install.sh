#!/bin/bash

total_steps=10

bar_width=50

bar_char="#"
empty_char="-"

function update_bar() {
  # Calculate the percentage of completion
  completed_steps=$1
  percentage=$((completed_steps * 100 / total_steps))

  # Calculate the number of characters to draw in the bar
  bar_size=$((bar_width * completed_steps / total_steps))

  # Draw the loading bar
  bar=$(printf "[%-${bar_width}s]" "${bar_char:0:bar_size}")
    empty=$(printf "[%-${bar_width}s]" "${empty_char:0:$((bar_width - bar_size))}")
    printf "\033[2K\r%s %d%% %s" "$bar" "$percentage" "$empty"
}

# Define the function to call when a step is completed
function completedStep() {
  # Increment the completed steps counter
  completed_steps=$((completed_steps + 1))

  # Clear the previous loading bar
  printf "\033[2K\r"

  # Update the loading bar
  update_bar "$completed_steps"

  # Check if all steps have been completed
  if [ "$completed_steps" -eq "$total_steps" ]; then
    echo ""
    echo "Task completed!"
  fi
}

function installTouchScreen {
  echo "=== Configuration of Touchscreen ==="



  if [ -f "/boot/config.txt" ]; then
    echo "#Joy-IT Touchscreen configuration"
    echo "dtparam=spi=on" >> /boot/config.txt
    echo "dtoverlay=joy-IT-Display-Driver-32b-overlay:rotate=270,swapxy=1" >> /boot/config.txt
    echo "dtparam=audio=on" >> /boot/config.txt
    echo "max_usb_current=1" >> /boot/config.txt
    echo "hdmi_force_hotplug=1" >> /boot/config.txt
    echo "config_hdmi_boost=7" >> /boot/config.txt
    echo "hdmi_drive=1" >> /boot/config.txt
    echo "hdmi_ignore_edid=0xa5000080" >> /boot/config.txt

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

  echo "Section "InputClass"" >> /usr/share/X11/xorg.conf.d/99-calibration.conf
  echo   "Identifier "calibration"" >> /usr/share/X11/xorg.conf.d/99-calibration.conf
  echo   "MatchProduct "ADS7846 Touchscreen"" >> /usr/share/X11/xorg.conf.d/99-calibration.conf
  echo   "Option "Calibration" "160 3723 3896 181"" >> /usr/share/X11/xorg.conf.d/99-calibration.conf
  echo   "Option "SwapAxes" "0"" >> /usr/share/X11/xorg.conf.d/99-calibration.conf
  echo "EndSection" >> /usr/share/X11/xorg.conf.d/99-calibration.conf

}

function enableAutoStartup {
  echo "=== Configuration of Autostart ==="

  mkdir /home/pi/.config/autostart

  echo "[Desktop Entry]" >> /home/pi/.config/autostart/duckyDetector.desktop
  echo "Name=DuckyDetector" >> /home/pi/.config/autostart/duckyDetector.desktop
  echo "Exec=sudo /home/pi/ducky-detector/DuckyDetector" >> /home/pi/.config/autostart/duckyDetector.desktop
  echo "Type=Application" >> /home/pi/.config/autostart/duckyDetector.desktop

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


  sudo apt-get update &>> ./debug.txt
  completedStep
  sudo apt-get upgrade &>> ./debug.txt
  completedStep

  sudo apt-get --assume-yes install g++ &>> ./debug.txt
  completedStep
  sudo apt-get --assume-yes install clamav &>> ./debug.txt
  completedStep
  sudo apt-get --assume-yes install libclamav-dev &>> ./debug.txt
  completedStep
  sudo apt-get --assume-yes install llibgtkmm-3.0-dev &>> ./debug.txt
  completedStep


  mkdir -p /mnt/mount
  completedStep
  sudo make &>> ./debug.txt
  completedStep



  echo "=== Installation Complete. Do you want to configure the touchscreen display? ==="
  echo "(1) yes"
  echo "(2) no"
  read -rp "> " answerTouchscreen

  if [ "$answerTouchscreen" = 1 ]; then
    installTouchScreen
  else
    echo "Proceeding to automatic Start"
  fi
  completedStep

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
  completedStep
fi

