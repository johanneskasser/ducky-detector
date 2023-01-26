#include "PrintingDetailsService.hpp"

PrintingDetailsService::PrintingDetailsService() = default;

bool PrintingDetailsService::printDetailsQuestion() {
    std::cout << " Do you want to see the details?" << std::endl;
    return PrintingService::checkInput() == 0;
}

std::string PrintingDetailsService::printDeviceInfos(Device& device) {
    std::string deviceInfo;

    if (device.getInitializationStatus()) {
            deviceInfo += "Vendor\n" + device.getVendorName(device.getIdVendor()) + "\\\\";
            deviceInfo += "IDVendor (Dec/Hex)\n" + std::to_string(device.getIdVendor()) + " / " + device.getIdVendorAsHex() + "\\\\";
            deviceInfo += "bNumInterfaces\n" + std::to_string(device.getbNumInterfaces()) + "\\\\";
            deviceInfo += "Device Class\n" + std::to_string(device.getbDeviceClass()) + "\\\\";
            deviceInfo += " - Product: " + device.getProductName(device.getIdVendor(), device.getIdProduct()) + "\n";
            deviceInfo += " - IDProduct (Dec/Hex): " + std::to_string(device.getIdProduct()) + " / " + device.getIdProductAsHex() + "\n";
            deviceInfo += " - DeviceSubClass: " + std::to_string(device.getbDeviceSubClass()) + "\n";
            deviceInfo += " - Device Protocol: " + std::to_string(device.getbDeviceProtocol()) + "\n";
            deviceInfo += " - bNumConfiguration: " + std::to_string(device.getbNumConfigurations()) + "\n";
            deviceInfo += " - Auth Status: " + std::to_string(device.getDeviceAuthorizationStatus()) + "\n";
            deviceInfo += " - path: " + device.getDevicePath() + "\n";

            int interfaces[device.getbNumInterfaces()];
            int *ifaces = device.getbNumInterfaceClasses(interfaces);

            for (int i = 0; i < device.getbNumInterfaces(); i++) {
                deviceInfo += " - Interface " + std::to_string(i) + ": " + std::to_string(ifaces[i]) + "\n";
                deviceInfo += " - Interface path: " + device.getInterfacePath(i) + "\n";
                deviceInfo += " - Interface status: " + std::to_string(device.getInterfaceAuthorizationStatus(i)) + "\n";
            }
    } else {
        deviceInfo += " - No Device detected yet! \n";
    }

    return deviceInfo;
}

std::string PrintingDetailsService::getPreparationDetailsForGui(const std::map<std::string, bool>& preparationAnalysisDetails) {
    std::string preparationDetails;

    if (preparationAnalysisDetails.at("modProbeDisabled")) {
        preparationDetails += "Drivers_autoprobe \n disabled \\\\";
    } else {
        preparationDetails += "Drivers_autoprobe \n enabled \\\\";
    }

    if (preparationAnalysisDetails.at("defaultAuthDisabled")) {
        preparationDetails += "Authorized_default \n disabled \\\\";
    } else {
        preparationDetails += "authorized_default \n enabled \\\\";
    }

    if (preparationAnalysisDetails.at("defaultInterfaceAuthDisabled")) {
        preparationDetails += "interface_authorized_default \n disabled \\\\";
    } else {
        preparationDetails += "interface_authorized_default \n enabled \\\\";
    }

    return preparationDetails;
}

void PrintingDetailsService::printPreparationDetailsForTerminal(const std::map<std::string, bool>& preparationAnalysisDetails) {
    printDetailsSignForTerminal();
    std::cout << getPreparationDetailsForGui(preparationAnalysisDetails) << std::endl;
}

std::string PrintingDetailsService::getPeripheryAnalysisDetailsForGui(const std::map<std::string, bool>& peripheryAnalysisDetails,
        Device& device) {
    std::string peripheryDetails;

    if (peripheryAnalysisDetails.at("deviceFound") && !peripheryAnalysisDetails.at("multipleDevicesFound")) {
        peripheryDetails += printDeviceInfos(device);
    }

    if (peripheryAnalysisDetails.at("deviceFound")) {
        peripheryDetails += " - Device detected\n";
    } else {
        peripheryDetails += " - Device could not be detected.\n";
    }

    if (peripheryAnalysisDetails.at("multipleDevicesFound")) {
        peripheryDetails += " - Multiple devices detected at the same time.\n";
    } else {
        peripheryDetails += " - No multiple devices detected at the same time.\n";
    }

    if (peripheryAnalysisDetails.at("deviceParameterChanged")) {
        peripheryDetails += " - Device parameters changed.\n";
    } else {
        peripheryDetails += " - Device parameters did not changed.\n";
    }

    if (peripheryAnalysisDetails.at("deviceVanished")) {
        peripheryDetails += " - Device detected but it vanished or it was removed.\n";
    } else {
        peripheryDetails += " - Device detected and did not vanished or was removed.\n";
    }

    if (peripheryAnalysisDetails.at("multipleInterfacesFound")) {
        peripheryDetails += " - Zero or more then one interface(s) detected.\n";
    } else {
        peripheryDetails += " - Exactly one interface was detected.\n";
    }

    if (peripheryAnalysisDetails.at("peripheryErrors")) {
        peripheryDetails += " - Library error occured.\n";
    } else {
        peripheryDetails += " - No library errors occured durring analysis process.\n";
    }

    return peripheryDetails;
}

void PrintingDetailsService::printUsbPeripheryAnalysisDetailsForTerminal(const std::map<std::string, bool>& peripheryAnalysisDetails,
        Device& device) {
    printDetailsSignForTerminal();
    std::cout << getPeripheryAnalysisDetailsForGui(peripheryAnalysisDetails, device) << std::endl;
    PrintingService::printConfirmForTerminal();
}

std::string PrintingDetailsService::getPartitionAnalysisDetailsForGui(int partitionCounter, const std::string *fileSystems,
        std::string* flags) {
    bool blackListedFileSystemOrFlag = false;
    std::string partitionDetails;

    if (partitionCounter > 1) {
        partitionDetails += "Detected Partitions\n " + std::to_string(partitionCounter) + "\\\\";
    } else {
        partitionDetails += "Detected Partitions\n " + std::to_string(partitionCounter) + "\\\\";
    }

    partitionDetails += " - Blacklisted file systems and flags \n efi, ext1, ext2, ext3, ext4,\n";
    partitionDetails += " boot, bios_grub, hidden, legacy_boot\\\\";

    for (int i = 0; i < partitionCounter; i++) {
        if (fileSystems[i] == "efi") {
            partitionDetails += " - EFI detected! \n USB drive may contain bootable content!\\\\";
            blackListedFileSystemOrFlag = true;
        }
        if (fileSystems[i] == "ext2") {
            partitionDetails += " - ext2 detected! \n USB drive contains a linux filesystem!\\\\";
            blackListedFileSystemOrFlag = true;
        }
        if (fileSystems[i] == "ext3") {
            partitionDetails += " - ext3 detected! \n USB drive contains a linux filesystem!\\\\";
            blackListedFileSystemOrFlag = true;
        }
        if (fileSystems[i] == "ext4") {
            partitionDetails += " - ext4 detected! \n USB drive contains a linux filesystem!\\\\";
            blackListedFileSystemOrFlag = true;
        }
        if (flags[i].find("boot") != std::string::npos) {
            partitionDetails += " - boot flag detected! \n USB drive may contain bootable content!\\\\";
            blackListedFileSystemOrFlag = true;
        }
        if (flags[i].find("bios_grub") != std::string::npos) {
            partitionDetails += " - bios_grub flag detected! \n USB drive may contain bootable content!\\\\";
            blackListedFileSystemOrFlag = true;
        }
        if (flags[i].find("hidden") != std::string::npos) {
            partitionDetails += " - hidden flag detected! \n USB drive tries to hide a partition!\\\\";
            blackListedFileSystemOrFlag = true;
        }
        if (flags[i].find("legacy_boot") != std::string::npos) {
            partitionDetails += " - legacy_boot flag detected! \n USB drive may contain bootable content!\\\\";
            blackListedFileSystemOrFlag = true;
        }
    }

    if (!blackListedFileSystemOrFlag) {
        partitionDetails += "Black listed file system or flags \n no \\\\";
    }

    return partitionDetails;
}

void PrintingDetailsService::printPartitionAnalysisDetailsForTerminal(int partitionCounter, const std::string* fileSystems,
        std::string* flags) {
    printDetailsSignForTerminal();
    std::cout << getPartitionAnalysisDetailsForGui(partitionCounter, fileSystems, flags) << std::endl;
    PrintingService::printConfirmForTerminal();
}

std::string PrintingDetailsService::getFileExtensionAnalysisDetailsForGui(int fileCounter, const std::set<std::string>& blackListedFiles) {
    std::string fileExtensionDetails;

    fileExtensionDetails += "Detected Files\n" + std::to_string(fileCounter) + "\\\\";
    fileExtensionDetails += " - Black listed file extensions: ";
    for (const auto& item : Settings().fileExtensionBlacklist) {
        fileExtensionDetails += item + ", ";
    }
    fileExtensionDetails.pop_back();
    fileExtensionDetails.push_back('\n');

    if (blackListedFiles.empty()) {
        fileExtensionDetails += "Black Listed Files\n no \\\\";
    } else {
        fileExtensionDetails += "Black Listed Files\n";
        for (const auto & blackListedFile : blackListedFiles) {
            fileExtensionDetails += " -- " + blackListedFile + "\n";
        }
        fileExtensionDetails += "\\\\";
    }

    return fileExtensionDetails;
}

void PrintingDetailsService::printFileExtensionAnalysisDetailsForTerminal(int fileCounter, const std::set<std::string> &blackListedFiles) {
    printDetailsSignForTerminal();
    std::cout << getFileExtensionAnalysisDetailsForGui(fileCounter, blackListedFiles) << std::endl;
    PrintingService::printConfirmForTerminal();
}

std::string PrintingDetailsService::getVirusAnalysisDetailsForGui(int fileCounter, unsigned int signature, long double scannedData,
        int infectedFilesCounter, const std::set<std::string>& virusNames, const std::set<std::string>& infectedFiles,
        std::map<std::string, bool> engineStatus) {
    std::string virusDetails;

    if (engineStatus.at("isClamAvInitialized")) {
        virusDetails += " - Clam Av Library initialisation successful.\n";
    } else {
        virusDetails += " - Clam Av Library initialisation failed.\n";
    }

    if (engineStatus.at("isEngineCreated")) {
        virusDetails += " - Engine creation successful.\n";
    } else {
        virusDetails += " - Engine creation failed.\n";
    }

    if (engineStatus.at("isDbInitialized")) {
        virusDetails += " - Database initialisation successful.\n";
    } else {
        virusDetails += " - Database initialisation failed.\n";
    }

    if (engineStatus.at("isEngineCompiled")) {
        virusDetails += " - Engine compilation successful.\n";
    } else {
        virusDetails += " - Engine compilation failed.\n";
    }

    virusDetails += "Scanned Files\n" + std::to_string(fileCounter) + "\\\\";
    virusDetails += "Loaded Signatures\n" + std::to_string(signature) + "\\\\";
    virusDetails += "MB scanned" + std::to_string(scannedData) + "\\\\";
    virusDetails += "Infected Files" + std::to_string(infectedFilesCounter) + "\\\\";

    if (infectedFilesCounter > 0) {
        virusDetails += " - Following viruses were detected:\n";
        for (const auto &virusName : virusNames) {
            virusDetails += " -- " + virusName + "\n";
        }

        virusDetails += " - Following files are infected:\n";
        for (const auto &infectedFile : infectedFiles) {
            virusDetails += " -- " + infectedFile + "\n";
        }
    }

    return virusDetails;
}

void PrintingDetailsService::printVirusAnalysisDetailsForTerminal(int fileCounter, unsigned int signature, long double scannedData,
        int infectedFilesCounter, std::set<std::string> virusNames, std::set<std::string> infectedFiles,
        std::map<std::string, bool> engineStatus) {
    printDetailsSignForTerminal();
    std::cout << getVirusAnalysisDetailsForGui(fileCounter, signature, scannedData, infectedFilesCounter, std::move(virusNames),
        std::move(infectedFiles), engineStatus) << std::endl;
    PrintingService::printConfirmForTerminal();
}

void PrintingDetailsService::printDetailsSignForTerminal() {
    std::string detailSign;

    detailSign += "                   ┌-------------------┐ \n";
    detailSign += "                   |      Details      | \n";
    detailSign += "                   └-------------------┘ \n";

    // clear terminal with ANSI escape codes
    std::cout << "\033[2J\033[1;1H";
    std::cout << detailSign << std::endl;
}

PrintingDetailsService::~PrintingDetailsService() = default;
