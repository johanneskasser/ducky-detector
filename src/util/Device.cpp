#include "Device.hpp"

Device::Device() {
    // counter increases by adding interfaces
	interfaceCounter = 0;
    // device is not set
	initializationStatus = false;
}

bool Device::authorizeAll() {
    setDeviceAuthorizationStatus(true);
    for (int i = 0; i < bNumInterfaces; i++) {
        // ATTENTION: only one configuration per interface supported!
        return setInterfaceAuthorizationStatus(i, true);
    }
    return false;
}

bool Device::addInterface(int bInterfaceClass, int bNumEndpoints) {
    if (bNumInterfaces > interfaceCounter) {
        interfaces[interfaceCounter].addInterface(bInterfaceClass, bNumEndpoints);
        interfaceCounter++;
        return true;
    } else {
        return false;
    }
}

bool Device::bindUsbStorageDriverToInterface(uint8_t interfaceId) {
    std::string localDevicePath;

    getDevicePathBasedOnUsbVer(localDevicePath);
    localDevicePath += ":" +  std::to_string((int)(unsigned)bNumConfigurations) + "." + std::to_string((int)(unsigned)interfaceId);

    bool result = FileHandler::writeStringToFile(settings.driversUsbStorageBindFilePath, localDevicePath.c_str());
    // wait for the system binding the driver
    usleep(settings.sleepTimeBinding);

    return  result;
}

bool Device::compareInterfaces(int bInterfaceClass, int bNumEndpoints) {
    bool match = false;
    for (int i = 0; i < bNumInterfaces; i++) {
        if (interfaces[i].compareInterface(bInterfaceClass, bNumEndpoints)) {
            match = true;
        }
    }
    return match;
}

// compares if a given device already exists
bool Device::compareDevice(int pIdVendor, int pIdProduct, int pbDeviceClass, int pbDeviceSubClass, int pbDeviceProtocol,
        int pDevicePathLen, int pbNumConfigurations, int pbNumInterfaces) const {
    // no device configurations is set atm
    if (!initializationStatus) {
        return false;
    }

    bool devicePathLenChangeLegitimate = false;
    if (this->devicePathLen != pDevicePathLen) {
        // PortLen for USB 2.0 == 1; for USB 3.0 == 2
        if (pDevicePathLen == 1 || pDevicePathLen == 2) {
            devicePathLenChangeLegitimate = true;
        }
    } else {
        devicePathLenChangeLegitimate = true;
    }

    return this->idVendor == pIdVendor
           && this->idProduct == pIdProduct
           && this->bDeviceClass == pbDeviceClass
           && this->bDeviceSubClass == pbDeviceSubClass
           && this->bDeviceProtocol == pbDeviceProtocol
           && this->bNumConfigurations == pbNumConfigurations
           && this->bNumInterfaces == pbNumInterfaces
           && devicePathLenChangeLegitimate;
}

std::string Device::decToHex(int dec) {
    std::stringstream stringStream;
    // int decimal_value
    stringStream << std::hex << dec;
    return stringStream.str();
}

bool Device::handleUsbMounting(bool mounting, const std::string &partition, const std::string &filesystem) {
    if (mounting) {
        return mountUsb(partition, filesystem);
    } else {
        return unmountUsb();
    }
}

bool Device::setDescriptor(int pIdVendor, int pIdProduct, int pbDeviceClass, int pbDeviceSubClass, int pbDeviceProtocol,
        int pDevicePathLen, int pbNumConfigurations, int pbNumInterfaces, const uint8_t pDevicePath[7]) {
    // device is already set
    if (initializationStatus) {
        return false;
    }

    updateDescriptor(pIdVendor, pIdProduct, pbDeviceClass, pbDeviceSubClass, pbDeviceProtocol, pDevicePathLen, pbNumConfigurations,
            pbNumInterfaces, pDevicePath);

    // device is set
    initializationStatus = true;

    return true;
}

void Device::updateDescriptor(int pIdVendor, int pIdProduct, int pbDeviceClass, int pbDeviceSubClass, int pbDeviceProtocol,
        int pDevicePathLen, int pbNumConfigurations, int pbNumInterfaces, const uint8_t pDevicePath[7]) {
    idVendor = pIdVendor;
    idProduct = pIdProduct;
    bDeviceClass = pbDeviceClass;
    bDeviceSubClass = pbDeviceSubClass;
    bDeviceProtocol = pbDeviceProtocol;
    devicePathLen = pDevicePathLen;
    bNumConfigurations = pbNumConfigurations;
    bNumInterfaces = pbNumInterfaces;
    for (int i = 0; i < 7; i++) {
        devicePath[i] = (unsigned)pDevicePath[i];
    }
}

// Getter
std::string Device::getDevicePath() {
    std::string temp = settings.sysfsPath;
    getDevicePathBasedOnUsbVer(temp);
    return temp;
}

std::string Device::getInterfacePath(uint8_t interfaceId) {
    std::string temp = getDevicePath();
    temp = temp + ":" +  std::to_string((int)(unsigned)bNumConfigurations) + "." + std::to_string((int)(unsigned)interfaceId);
    return temp;
}

bool Device::getDeviceAuthorizationStatus() {
    return FileHandler::readBitFile((getDevicePath() + "/authorized").c_str());
}

bool Device::getInterfaceAuthorizationStatus(uint8_t interfaceId) {
    return FileHandler::readBitFile((getInterfacePath(interfaceId) + "/authorized").c_str());
}

bool Device::getInitializationStatus() {
	return initializationStatus;
}

void Device::setInitializationStatus(bool status) {
    initializationStatus = status;
}

int Device::getIdVendor() {
	return idVendor;
}

std::string Device::getIdVendorAsHex() {
    return decToHex(idVendor);
}

int Device::getIdProduct() {
	return idProduct;
}

std::string Device::getIdProductAsHex() {
    return decToHex(idProduct);
}

int Device::getbNumConfigurations() {
	return bNumConfigurations;
}

int Device::getbNumInterfaces() {
	return bNumInterfaces;
}

int Device::getbDeviceClass() {
	return bDeviceClass;
}

int Device::getbDeviceSubClass() {
	return bDeviceSubClass;
}

int Device::getbDeviceProtocol() {
	return bDeviceProtocol;
}

int* Device::getbNumInterfaceClasses(int ifaces[]) {
    for (int i = 0; i < bNumInterfaces; i++) {
        ifaces[i] = interfaces[i].getbInterfaceClass();
    }
    return ifaces;
}

std::string Device::getVendorName(int vendorId) {
    FILE* fp = fopen(settings.usbIdPath, "r");

    if (fp == NULL)
        exit(EXIT_FAILURE);

    char* line = NULL;
    size_t len = 0;

    std::string vendorIdfixed = fixStringLength(decToHex(vendorId));

    while ((getline(&line, &len, fp)) != -1) {
        // using printf() in all tests for consistency
        bool hit = true;

        for (int i = 0; i < 4; i++) {
            if (line[i] != vendorIdfixed[i]) {
                hit = false;
            }
        }

        if (hit) {
            std::string vendorName(line);
            if (line) {
                free(line);
            }

            fclose(fp);

            // cut 4 ID digits - 2 spaces & cut the newline at the end!
            return vendorName.substr(4 + 2, vendorName.length() - 4 - 2 - 1);
        }

    }

    fclose(fp);
    if (line) {
        free(line);
    }

    return "";
}

std::string Device::getProductName(int vendorId, int productId) {
    // we need both, because product ids are not unique
    // first search for the vendor-block, then for the product itself
    FILE* fp = fopen(settings.usbIdPath, "r");

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    char* line = NULL;
    size_t len = 0;

    std::string vendorIdfixed = fixStringLength(decToHex(vendorId));
    std::string productIdfixed = fixStringLength(decToHex(productId));

    while ((getline(&line, &len, fp)) != -1) {
        bool hit = true;
        bool hit2;

        for (int i = 0; i < 4; i++) {
            if (line[i] != vendorIdfixed[i]) {
                hit = false;
            }
        }

        if (hit) {
            do {
                // get next line
                getline(&line, &len, fp);
                hit2 = true;

                for (int x = 0; x < 4; x++) {
                    // +1 because every line has one tabulator
                    if (line[x + 1] != productIdfixed[x]) {
                        hit2 = false;
                    }
                }

                if (hit2) {
                    std::string productName(line);
                    fclose(fp);

                    if (line) {
                        free(line);
                    }

                    // cut TAB - 4 ID digits - two spaces & cut the newline at the end!
                    return productName.substr(1 + 4 + 2, productName.length() - 1 - 4 - 2 - 1);
                }
            } while (line[0] == 9); // ASCII Code 9 = tabulator
        }
    }
    fclose(fp);

    if (line) {
        free(line);
    }

    return "";
}

// Private--------------------------------------------------------------------------------------------------------------
std::string Device::fixStringLength(std::string vendorId) {
    std::string vendorIdFixed = "";

    if (vendorId.length() == 4) {
        vendorIdFixed.append(vendorId);
    }

    if(vendorId.length() == 3) {
        vendorIdFixed.append("0");
        vendorIdFixed.append(vendorId);
    }

    if(vendorId.length() == 2) {
        vendorIdFixed.append("00");
        vendorIdFixed.append(vendorId);
    }

    if(vendorId.length() == 1) {
        vendorIdFixed.append("000");
        vendorIdFixed.append(vendorId);
    }

    return vendorIdFixed;
}

std::string Device::getDevicePathBasedOnUsbVer(std::string& path) {
    // Device path len == 1 -> USB 3.0, therefore using USB Bus 2
    // Device path len == 2 -> USB 2.0, therefore using USB Bus 1
    if (devicePathLen == 1) {
        path += "2-";
    } else if (devicePathLen == 2) {
        path += "1-";
    }

    for (int i = 0; i < (unsigned)devicePathLen; i++) {
        path += std::to_string((int)(unsigned)devicePath[i]);
        if (i < (unsigned)devicePathLen - 1) {
            path += ".";
        }
    }

    return path;
}

bool Device::mountUsb(const std::string& partition, const std::string& filesystem) {
    unsigned long mountflags = MS_NOATIME | MS_RDONLY;
    std::string filesystem_ = filesystem;
    // often the partion ID is only "/sda" without a Number
    std::string partitionIDShort = partition.substr(0, partition.size()-1);
    // in some cases the ID is "/sda1"
    std::string partitionIDLong  = partition;

    std::cout<< "Partition: " << partition << std::endl;

    if (filesystem == "fat16") {
        filesystem_ = "vfat";
        // readonly on fat16 didnt work well in the past
        mountflags = MS_NOATIME;
    } else if (filesystem.find("fat") != std::string::npos) {
        filesystem_ = "vfat";
    }

    // first we are trying the short version
    // if not successful, try the long version
    // errors can be caught like: if(errno == EBUSY). print errors: printf("Mount error: %s", strerror(errno));
    if (mount(partitionIDShort.c_str(), settings.mountPoint, filesystem_.c_str(), mountflags, nullptr) == 0) {
        printf("Mount error: %s\n", strerror(errno));
        return true;
    } else {
        printf("Mount error: %s\n", strerror(errno));
        return mount(partitionIDLong.c_str(), settings.mountPoint, filesystem_.c_str(), mountflags, nullptr) == 0;
    }
}

bool Device::unmountUsb() {
    // "On success, zero is returned. On error, -1 is returned, and errno is set appropriately."
    usleep(settings.sleepTimeBinding);
    return umount(settings.mountPoint) == 0;
}

bool Device::setDeviceAuthorizationStatus(bool state) {
    return FileHandler::writeBitFile((getDevicePath() + "/authorized").c_str(), state);
}

bool Device::setInterfaceAuthorizationStatus(uint8_t interfaceId, bool state) {
    return FileHandler::writeBitFile((getInterfacePath(interfaceId) + "/authorized").c_str(), state);
}

bool Device::detectDevice(libusb_device *device) {
    libusb_device_descriptor deviceDescriptor;
    bool portInUse = libusb_get_port_number(device) != 0;
    bool deviceDescriptorExists = libusb_get_device_descriptor(device, &deviceDescriptor) == 0;
    bool isUnderlyingUsbHub = deviceDescriptor.idVendor == settings.viaLabsHubVendorId && deviceDescriptor.idProduct == settings.viaLabsHubProductId;

    return portInUse && deviceDescriptorExists && !isUnderlyingUsbHub;
}

bool Device::compareArray_uint8_t(const uint8_t *arr1, const uint8_t *arr2, uint8_t size) {
    for (int i = 0; i < size; ++i) {
        if (arr1[i] != arr2[i]) {
            return false;
        }
    }
    return true;
}

Device::~Device() = default;
