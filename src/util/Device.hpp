#ifndef DEVICE_H // prevents multiple compiles
#define DEVICE_H

#include <cstdint>
#include <iostream>
#include <libusb-1.0/libusb.h>
#include <sstream>
#include <string>
#include <sys/mount.h>
#include <unistd.h>
#include "FileHandler.hpp"
#include "Interface.hpp"
#include "cstring"
#include "../config/Settings.hpp"

class Device {
    public:
        Device();
        virtual ~Device();

        bool authorizeAll();
        bool addInterface(int bInterfaceClass, int bNumEndpoints);
        bool bindUsbStorageDriverToInterface(uint8_t interfaceId);
        // cycles through stored interfaces and looks for differences
        bool compareInterfaces(int bInterfaceClass, int bNumEndpoints);
        bool compareDevice(int idVendor, int idProduct, int bDeviceClass, int bDeviceSubClass, int bDeviceProtocol,
                           int devicePathLen, int bNumConfigurations, int bNumInterfaces) const;
        bool handleUsbMounting(bool mounting, const std::string& partition, const std::string& filesystem);
        bool setDescriptor(int idVendor, int idProduct, int bDeviceClass, int bDeviceSubClass, int bDeviceProtocol,
            int devicePathLen, int bNumConfigurations, int bNumInterfaces, const uint8_t devicePath[7]);
        void updateDescriptor(int idVendor, int idProduct, int bDeviceClass, int bDeviceSubClass, int bDeviceProtocol,
            int devicePathLen, int bNumConfigurations, int bNumInterfaces, const uint8_t devicePath[7]);

        // TODO: replace specific VIA Labs, Inc. Hub (ID 2109:3431) identification
        bool detectDevice(libusb_device *device);
        // maybe replacing detectDevice() as already in the first version
        bool compareArray_uint8_t(uint8_t const arr1[], uint8_t const arr2[], uint8_t size);

        // Getter & Setter
        std::string getDevicePath();
        std::string getInterfacePath(uint8_t interfaceId);
        bool getDeviceAuthorizationStatus();
        bool getInterfaceAuthorizationStatus(uint8_t interfaceId);
        bool getInitializationStatus();
        void setInitializationStatus(bool status);
        int getIdVendor();
        std::string getIdVendorAsHex();
        int getIdProduct();
        std::string getIdProductAsHex();
        int getbNumConfigurations();
        int getbNumInterfaces();
        int getbDeviceClass();
        int getbDeviceSubClass();
        int getbDeviceProtocol();
        // returns an array with interfaces class ids
        int* getbNumInterfaceClasses(int ifaces[]);
        std::string getVendorName(int vendorId);
        std::string getProductName(int vendorId, int productId);

    private:
        Settings settings;
        // state if device is already set or not
        bool initializationStatus;

        int idVendor;
        int idProduct;
        int bDeviceClass;
        int bDeviceSubClass;
        int bDeviceProtocol;
        int devicePath[7];
        int devicePathLen;

        Interface interfaces[20];
        int interfaceCounter;
        int bNumConfigurations;
        int bNumInterfaces;

        std::string decToHex(int dec);
        std::string fixStringLength(std::string vendorId);
        std::string getDevicePathBasedOnUsbVer(std::string& path);
        bool mountUsb(const std::string& partition, const std::string& filesystem);
        bool unmountUsb();
        bool setDeviceAuthorizationStatus(bool state);
        bool setInterfaceAuthorizationStatus(uint8_t interfaceId, bool state);
};

#endif
