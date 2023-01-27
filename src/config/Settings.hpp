#ifndef SETTINGS_H // prevents multiple compiles
#define SETTINGS_H

#include <string>

class Settings {
    public:
        // Paths
        const char* mountPoint = "/media/pi";
        const char* sysfsPath = "/sys/bus/usb/devices/";
        const char* usbIdPath = "/usr/share/misc/usb.ids";
        // Files
        const char* driversUsbStorageBindFilePath = "/sys/bus/usb/drivers/usb-storage/bind";
        const char* modProbeFilePath = "/sys/bus/usb/drivers_autoprobe";
        const char* defaultAuthFilePathUsbBus1 = "/sys/bus/usb/devices/usb1/authorized_default";
        const char* defaultAuthFilePathUsbBus2 = "/sys/bus/usb/devices/usb2/authorized_default";
        const char* defaultInterfaceAuthFilePathUsbBus1 = "/sys/bus/usb/devices/usb1/interface_authorized_default";
        const char* defaultInterfaceAuthFilePathUsbBus2 = "/sys/bus/usb/devices/usb2/interface_authorized_default";
        // Underlying USB Hub of Raspi
        // IDs found with lsusb
        const uint16_t viaLabsHubVendorId = 8457;
        const uint16_t viaLabsHubProductId = 13361;
        // Time Settings
        const int stage0Timeout = 5;
        const int stage1Timeout = 2;
        const unsigned int sleepTimeBinding = 2000000;
        // Blacklist
        const std::string fileExtensionBlacklist[10] = {".exe", ".sh", ".bat", ".cmd", ".vb", ".vbs", ".js", ".ps1",
                                                        ".reg", ".vbe"};

        // port definitions for the raspberry pi v4
        //    const uint8_t port1[1] = {1};
        //    const uint8_t port2[1] = {2};
        //    const uint8_t port3[2] = {1, 3};
        //    const uint8_t port4[2] = {1, 4};

        // port definitions for the raspberry pi v3
        //    const uint8_t port1[3] = {1,1,2}; // first 3 tupel relevant
        //    const uint8_t port2[3] = {1,1,3};
        //    const uint8_t port3[2] = {1,3}; // first 2 tupel relevant - ports are on different levels registered
        //    const uint8_t port4[2] = {1,2};

        // port definitions for the raspberry pi v2
        //const uint8_t port1[3] = {1,2,0}; // first 2 tupel relevant
        //const uint8_t port2[3] = {1,3,0};
        //const uint8_t port3[4] = {1,4,0,0};
        //const uint8_t port4[4] = {1,5,0,0};

};
#endif