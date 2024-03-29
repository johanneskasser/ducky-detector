# Ducky Detector

Dieses Projekt erweitert den DuckyDetektor von Philipp Lohse und Dennis La um eine verbesserte grafische Oberfläche und eine stark vereinfachte Installation.

## Erste Schritte

Diese Instruktionen werden Dich zu einer funktionsfühigen Kopie dieses Projekts verschaffen, welches auf einer lokalen Umgebung für Entwicklungs- und  Testzwecke läuft. Für den Einsatz auf einem Live System wende Dich an den Abschnitt ["Schnelle Installation"](#Schnelle_Installation). Falls Du die Konfiguration lieber manuell übernehmen möchtest, wende Dich bitte an den Abschnitt ["Bereitstellung"](#Bereitstellung).

### Voraussetzung

- Raspberry Pi 4 Model B
    - min. 4 GB RAM
- Raspberry Pi OS (32-bit) mit Desktop
    - Version: min. Mai 2020
    - Release Datum: min. 2020-05-27
    - Kernel Version: min. 5.4
- Alternativ: Custom OS Image (siehe ["Schnelle Installation"](#Schnelle_Installation))
- Optional: 3,2"/3,5" Touchscreen Display
- Optional: Schutzhülle für den Raspberry Pi
- [Raspberry Pi Imager](https://www.raspberrypi.com/software/)


### <a name="Schnelle_Installation"></a>Schnelle Installation

Unter diesem [Link](https://drive.google.com/file/d/1rc4VAEZ_PBI_TmvLSsJv3klG4QjUyw9A/view?usp=sharing) ist ein Image verfügbar, auf dem der Rubber Ducky Detector mit konfiguriertem JoyIT-3,2"-Touchscreen vorinstalliert ist. Das Image kann mit dem [Raspberry Pi Imager](https://www.raspberrypi.com/software/) Tool auf eine SD-Karte übertragen werden. Unter dem Reiter "OS wählen" kann mit der Option "Eigenes Image" die bereitgestellte .img-Datei ausgewählt und geflashed werden.

![Raspberry Pi Imager](https://i.imgur.com/REnrDGa.png)

Die SD-Karte muss vor dem Start des RasPi in den Slot eingesteckt werden.

Credentials für img Datei:

Username: `pi`
Password: `duckyDetector`

#### Troubleshooting der schnellen Installation

Alternativ ist im Repository ein `install.sh` Skript enthalten, der automatisch die Konfiguration des Ducky Detectors und die Installation der notwendigen Abhängigkeiten vornimmt. Um den Skript ausführbar zu machen, muss zuvor der Befehl `sudo chmod +x ./install.sh` ausgeführt werden. Zur Ausführung des Skripts sind Root-Rechte nötig. Eventuelle Debug-Informationen sind der während der Laufzeit erstellten `debug.txt` Datei zu entnehmen.


### Manuelle Installation

- das Betriebssystem Raspberry Pi OS auf eine Speicherkarte flashen z.B. mit balenaEtcher
- um auf den Raspberry Pi über ssh zuzugreifen muss folgendes durchgeführt werden:
    - eine Datei mit dem Namen "ssh" ohne Inhalt erstellen 
    - eine Datei mit dem Namen "wpa_supplicant.conf" erstellen, mit dem folgenden Inhalt

```
country=AT
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1

network={
    ssid="NETWORK-NAME"
    psk="NETWORK-PASSWORD"
}
```

## <a name="Bereitstellung"></a> Bereitstellung

Folgende Schritte sind dazu nötig, um dieses Projekt auf einem Live System
einzusetzen.

- Optional: Pakete aktualisieren

```
sudo apt-get update
sudo apt-get upgrade
```

- Folgende Pakete müssen installiert werden:
    - g++
    - libusb-1.0-0-dev
    - libclamav-dev
    - libgtkmm-3.0-dev

```
sudo apt-get install g++ libusb-1.0-0-dev clamav libclamav-dev libgtkmm-3.0-dev
```

- einen Ordner "mount" in "/mnt" erstellen
- einen Ornder "duckyDetector" in "/home/pi" erstellen
    - und darin das Projekt abspeichern
- mittels `make` kann das Projekt kompiliert werden

### Optional: Touchscreen einstellen

Für dieses Projekt wurde ein 3,2" Touchscreen Display von joy-it verwendet.
Folgende Anleitung basiert auf die Verwendung eines 3,2" Touchscreen Displays von joy-it.

1. Datei config.txt bearbeiten
    - in dieser Datei muss folgender Inhalt eingefügt werden

    ```
    sudo nano /boot/config.txt
    
    dtparam=spi=on
    dtoverlay=joy-IT-Display-Driver-32b-overlay:rotate=270,swapxy=1
    dtparam=audio=on
    max_usb_current=1
    hdmi_force_hotplug=1
    config_hdmi_boost=7
    hdmi_drive=1
    hdmi_ignore_edid=0xa5000080
    ```

    Wird ein Raspberry Pi 4 verwendet muss zusätzlich folgende Zeile auskommentiert werden

    ```
    [pi4]
    # Enable DRM VC4 V3D driver on top of the dispmanx display stack
    #
    #dtoverlay=vc4-fkms-v3d <-- Diese Zeile auskommentieren
    max_framebuffers=2
    ```

2. Datei cmdline.txt bearbeiten
    - in dieser Datei muss in die erste Zeile folgendes eingfügt werden
    
   ```
   fbcon=map:10
   ```

3. Datei `99-calibration.conf` erstellen und folgendes einfügen

    ```
    sudo nano /usr/share/X11/xorg.conf.d/99-calibration.conf
    
    Section "InputClass"
        Identifier "calibration"
        MatchProduct "ADS7846 Touchscreen"
        Option "Calibration" "160 3723 3896 181"
        Option "SwapAxes" "0"
    EndSection
    ```

4. System neustarten
5. Datei `99-fbturbo.conf` bearbeiten

    ```
    sudo nano /usr/share/X11/xorg.conf.d/99-fbturbo.conf
    
    # Diese Zeile
    Option "fbdev" "/dev/fb0"
    
    # In diese umändern
    Option "fbdev" "/dev/fb1"
    ```

6. Benötigte Treiber herunterladen und installieren
    
    ```
    cd /tmp
   
    # Kommandos für den 3,2" Touchscreen Display
    wget http://joy-it.net/files/files/Produkte/RB-TFT3.2-V2/joy-IT-Display-Driver-32b-overlay.zip
    unzip joy-IT-Display-Driver-32b-overlay.zip
    sudo cp joy-IT-Display-Driver-32b-overlay.dtbo /boot/overlays/joy-IT-Display-Driver-32b-overlay.dtbo
   
    # Funktionalität für den 3,2" Touchscreen Display
    sudo apt-get install xserver-xorg-input-evdev
    sudo cp -rf /usr/share/X11/xorg.conf.d/10-evdev.conf /usr/share/X11/xorg.conf.d/45-evdev.conf
    ```

7. Automount deaktivieren
    - dazu die Datei `pcmanfm.conf` bearbeiten und folgendes einfügen
    
    ```
    sudo nano /home/pi/.config/pcmanfm/LXDE-pi/pcmanfm.conf
   
    # Diese Zeilen einfügen
    [volume]
    mount_on_startup=0
    mount_removable=0
    ```

8. System neustarten
9. Optional: Display kalibrieren
    - weitere Informationen können in der [Anleitung](https://joy-it.net/files/files/Produkte/RB-TFT3.2-V2/RB-TFT-Manual_04082020.pdf) gefunden werden

### Optional: Ausführen des Programmes nach Systemstart

Wird ein Display verwendet kann das Programm bereits nach dem Systemstart automatisch ausgeführt werden. Dazu muss folgendes getan werden:

- einen Ordner `autostart` erstellen

```
mkdir /home/pi/.config/autostart
```

- eine Datei `duckyDetector.desktop` erstellen mit folgendem Inhalt:

```
nano /home/pi/.config/autostart/duckyDetector.desktop

[Desktop Entry]
Type=Application
Name=DuckyDetector
Exec=sudo /home/pi/duckyDetector/DuckyDetector
```

## Implementiert mit

* [CLion](https://www.jetbrains.com/de-de/clion/) - IDEA
* [Glade](https://glade.gnome.org/) - Tool für GTK+ Interface Design
* [Make](https://www.gnu.org/software/make/) - Build Management Tool
* [libusb](https://libusb.info/) - Bibliothek für den Zugriff auf USB Geräte
* [ClamAv](https://www.clamav.net/) - Open Source Antivirus Engine
* [gtkmm](https://www.gtkmm.org/en/) - Offizielle C++ Interface für die GTK+ GUI Bibliothek

## Autoren

* **Philipp Lohse** - *Initiale Arbeit*
* **Dennis La** - *Weiterführende Arbeit*
* **Johannes Kasser** - *Usability-Verbesserungen*
* **Joseph Hangstein** - *Usability-Verbesserungen*

## Weitere Informationen
- für die Verwendung mit CLion wurden `CMakeLists.txt` Dateien erstellt
    - diese Dateien erlauben die Verwendung des code insight features

## Weiterführende Links
- [ssh und WIFI über Speicherkarte aktivieren](https://medium.com/@nikosmouroutis/how-to-setup-your-raspberry-pi-and-connect-to-it-through-ssh-and-your-local-wifi-ac53d3839be9)
- [Touchscreen einstellen](https://joy-it.net/files/files/Produkte/RB-TFT3.2-V2/RB-TFT-Manual_04082020.pdf)
- [Automount deaktivieren](https://www.raspberrypi.org/forums/viewtopic.php?t=91677)
- [Programm starten nach Systemstart](https://learn.sparkfun.com/tutorials/how-to-run-a-raspberry-pi-program-on-startup/all)

####### English below

# DuckyDetector version 

This project enhances the DuckyDetector by Philipp Lohse and Dennis La by introducing an improved User Interface and a significantly simplified setup process.

## Getting started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See ["Quick Start"](#Quick_Start) on how to deploy the project on a live system. If you prefer to configure the system manually, please refer to the section ["Manual Installation"](#Manual_Installation).

### Prerequisites

- Raspberry Pi 4 Model B
    - at least 4 GB RAM
- Raspberry Pi OS (32-bit) with desktop
    - Version: at least May 2020
    - Release date: at least 2020-05-27
    - Kernel version: at least 5.4
- Alternatively: Custom OS Image (for reference, see ["Quick Start"](#Quick_Start))
- Optional: 3,2"/3,5" Touchscreen Display
- Optional: Case for the Raspberry Pi
- [Raspberry Pi Imager](https://www.raspberrypi.com/software/)


### <a name="Quick_Start"></a>Quick Start

A custom image is available under this [link](https://drive.google.com/file/d/1rc4VAEZ_PBI_TmvLSsJv3klG4QjUyw9A/view?usp=sharing). The image contains a pre-installed version of the "Ducky Detector" utility, with the JoyIT 3,2" touchscreen already configured. The image can be flashed onto a SD card with the tool [Raspberry Pi Imager](https://www.raspberrypi.com/software/). The drop-down menu "Choose OS" contains the option "Custom Image" to choose flash the provided .img file.

![Raspberry Pi Imager](https://i.imgur.com/REnrDGa.png)

Before booting the Raspberry Pi, the SD card needs to be inserted into the SD slot.

Credentials for custom image:

Username: `pi`
Password: `duckyDetector`

#### Troubleshooting quick start

Alternatively, the repository contains the script `install.sh` that automatically configures the Ducky Detector utility and installs all necessary dependencies. To make the file executable, type `sudo chmod +x ./install.sh`. The user running the script needs root privilege. Debug informations are written to the file `debug.txt` that is created at runtime.

### <a name="Manual_Installation"></a>Manual Installation

- Flash the operating system Raspberry Pi OS on a memory card e.g. balenaEtcher
- To access the Raspberry Pi via ssh the following needs to be done:
    - Create a file "ssh" without content
    - Create a file "wpa_supplicant.conf" with the following content

```
country=AT
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1

network={
    ssid="NETWORK-NAME"
    psk="NETWORK-PASSWORD"
}
```

## Deployment

Following steps needs to be done to deploy this project on a live system.

- Optional: update and upgrad packages

```
sudo apt-get update
sudo apt-get upgrade
```

- Following packages are needed to be installed:
    - g++
    - libusb-1.0-0-dev
    - libclamav-dev
    - libgtkmm-3.0-dev

```
sudo apt-get install g++ libusb-1.0-0-dev clamav libclamav-dev libgtkmm-3.0-dev
```

- create a directory "mount" in "/mnt"
- create a directory "duckyDetector" in "/home/pi"
    - save the project in there
- compile the project with the command `make`

### Optional: Install Touchscreen

For this project a 3.2" Touchscreen Display of joy-it was used. The following
steps are based on the usage of the 3.2" Touchscreen Display of joy-it.

1. Edit the file config.txt
    - in this file the following content needs to be added

    ```
    sudo nano /boot/config.txt
    
    dtparam=spi=on
    dtoverlay=joy-IT-Display-Driver-32b-overlay:rotate=270,swapxy=1
    dtparam=audio=on
    max_usb_current=1
    hdmi_force_hotplug=1
    config_hdmi_boost=7
    hdmi_drive=1
    hdmi_ignore_edid=0xa5000080
    ```

    Is a Raspberry Pi 4 in use the following content needs to be commented out.

    ```
    [pi4]
    # Enable DRM VC4 V3D driver on top of the dispmanx display stack
    #
    #dtoverlay=vc4-fkms-v3d <-- This setting must be commented ou
    max_framebuffers=2
    ```

2. Edit the file cmdline.txt
    - in this file the following content needs to be added to the first line
    
   ```
   fbcon=map:10
   ```

3. Create a file `99-calibration.conf` and add the following content

    ```
    sudo nano /usr/share/X11/xorg.conf.d/99-calibration.conf
    
    Section "InputClass"
        Identifier "calibration"
        MatchProduct "ADS7846 Touchscreen"
        Option "Calibration" "160 3723 3896 181"
        Option "SwapAxes" "0"
    EndSection
    ```

4. Reboot system
5. Edit file `99-fbturbo.conf`

    ```
    sudo nano /usr/share/X11/xorg.conf.d/99-fbturbo.conf
    
    # Change this line
    Option "fbdev" "/dev/fb0"
    
    # to this
    Option "fbdev" "/dev/fb1"
    ```

6. Further drivers needs to be downloaded and installed
    
    ```
    cd /tmp
   
    # Commands for the 3,2" Touchscreen Display
    wget http://joy-it.net/files/files/Produkte/RB-TFT3.2-V2/joy-IT-Display-Driver-32b-overlay.zip
    unzip joy-IT-Display-Driver-32b-overlay.zip
    sudo cp joy-IT-Display-Driver-32b-overlay.dtbo /boot/overlays/joy-IT-Display-Driver-32b-overlay.dtbo
   
    # Functionality for the 3,2" Touchscreen Display
    sudo apt-get install xserver-xorg-input-evdev
    sudo cp -rf /usr/share/X11/xorg.conf.d/10-evdev.conf /usr/share/X11/xorg.conf.d/45-evdev.conf
    ```

7. Disable automount
    - therefore edit the file `pcmanfm.conf` and add the following
    
    ```
    sudo nano /home/pi/.config/pcmanfm/LXDE-pi/pcmanfm.conf
   
    # Diese Zeilen einfügen
    [volume]
    mount_on_startup=0
    mount_removable=0
    ```

8. Reboot system
9. Optional: Calibrate display
    - further information can be obtained [here](https://joy-it.net/files/files/Produkte/RB-TFT3.2-V2/RB-TFT-Manual_04082020.pdf)

### Optional: Run application after startup

If the application is used with a display the application can be started right after booting the system with the following steps:

- create a directory `autostart`

```
mkdir /home/pi/.config/autostart
```

- create a file `duckyDetector.desktop` with the following content:

```
nano /home/pi/.config/autostart/duckyDetector.desktop

[Desktop Entry]
Type=Application
Name=DuckyDetector
Exec=sudo /home/pi/duckyDetector/DuckyDetector
```

## Built With

* [CLion](https://www.jetbrains.com/clion/) - IDEA
* [Glade](https://glade.gnome.org/) - Tool to design GTK+ interfaces
* [Make](https://www.gnu.org/software/make/) - Build Management Tool
* [libusb](https://libusb.info/) - Library accessing USB devices
* [ClamAv](https://www.clamav.net/) - Open source antivirus engine
* [gtkmm](https://www.gtkmm.org/en/) - Official C++ interface for the GTK+ GUI Library

## Authors

* **Philipp Lohse** - *Initial work*
* **Dennis La** - *Further work*
* **Johannes Kasser** - *Usability improvements*
* **Joseph Hangstein** - *Usability improvements*

## Further information
- for the usage with CLion `CMakeLists.txt` files were created
    - these files allows the usage of `code insight feature`

## Further links
- [Enable ssh and wifi via memory card](https://medium.com/@nikosmouroutis/how-to-setup-your-raspberry-pi-and-connect-to-it-through-ssh-and-your-local-wifi-ac53d3839be9)
- [Install touchscreen](https://joy-it.net/files/files/Produkte/RB-TFT3.2-V2/RB-TFT-Manual_04082020.pdf)
- [Disable autmount](https://www.raspberrypi.org/forums/viewtopic.php?t=91677)
- [Run application on startup](https://learn.sparkfun.com/tutorials/how-to-run-a-raspberry-pi-program-on-startup/all)
