#include "PrintingDetailsService.hpp"

PrintingDetailsService::PrintingDetailsService() = default;

bool PrintingDetailsService::printDetailsQuestion() {
    std::cout << " Do you want to see the details?" << std::endl;
    return PrintingService::checkInput() == 0;
}

std::string PrintingDetailsService::printDeviceInfos(Device& device) {
    std::string deviceInfo;

    if (device.getInitializationStatus()) {
            deviceInfo += " - Vendor: " + device.getVendorName(device.getIdVendor()) + "\n";
            deviceInfo += " - IDVendor (Dec/Hex): " + std::to_string(device.getIdVendor()) + " / " + device.getIdVendorAsHex() + "\n";
            deviceInfo += " - Product: " + device.getProductName(device.getIdVendor(), device.getIdProduct()) + "\n";
            deviceInfo += " - IDProduct (Dec/Hex): " + std::to_string(device.getIdProduct()) + " / " + device.getIdProductAsHex() + "\n";
            deviceInfo += " - Device Class: " + std::to_string(device.getbDeviceClass()) + "\n";
            deviceInfo += " - DeviceSubClass: " + std::to_string(device.getbDeviceSubClass()) + "\n";
            deviceInfo += " - Device Protocol: " + std::to_string(device.getbDeviceProtocol()) + "\n";
            deviceInfo += " - bNumConfigurations: " + std::to_string(device.getbNumConfigurations()) + "\n";
            deviceInfo += " - Auth Status: " + std::to_string(device.getDeviceAuthorizationStatus()) + "\n";
            deviceInfo += " - path: " + device.getDevicePath() + "\n";
            deviceInfo += " - bNumInterfaces: " + std::to_string(device.getbNumInterfaces()) + "\n";

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
        preparationDetails += " - drivers_autoprobe is disabled.";
    } else {
        preparationDetails += " - drivers_autoprobe is enabled.";
    }

    if (preparationAnalysisDetails.at("defaultAuthDisabled")) {
        preparationDetails += " - authorized_default is disabled.";
    } else {
        preparationDetails += " - authorized_default is enabled.";
    }

    if (preparationAnalysisDetails.at("defaultInterfaceAuthDisabled")) {
        preparationDetails += " - interface_authorized_default is disabled.";
    } else {
        preparationDetails += " - interface_authorized_default is enabled.";
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
        partitionDetails += " - " + std::to_string(partitionCounter) + " partition/s detected. (Only one expected.)\n";
    } else {
        partitionDetails += " - " + std::to_string(partitionCounter) + " partition detected. \n";
    }

    partitionDetails += " - Blacklisted file systems and flags: efi, ext1, ext2, ext3, ext4,\n";
    partitionDetails += " boot, bios_grub, hidden, legacy_boot\n";

    for (int i = 0; i < partitionCounter; i++) {
        if (fileSystems[i] == "efi") {
            partitionDetails += " - EFI detected! --> USB drive may contain bootable content!\n";
            blackListedFileSystemOrFlag = true;
        }
        if (fileSystems[i] == "ext2") {
            partitionDetails += " - ext2 detected! --> USB drive contains a linux filesystem!\n";
            blackListedFileSystemOrFlag = true;
        }
        if (fileSystems[i] == "ext3") {
            partitionDetails += " - ext3 detected! --> USB drive contains a linux filesystem!\n";
            blackListedFileSystemOrFlag = true;
        }
        if (fileSystems[i] == "ext4") {
            partitionDetails += " - ext4 detected! --> USB drive contains a linux filesystem!\n";
            blackListedFileSystemOrFlag = true;
        }
        if (flags[i].find("boot") != std::string::npos) {
            partitionDetails += " - boot flag detected! --> USB drive may contain bootable content!\n";
            blackListedFileSystemOrFlag = true;
        }
        if (flags[i].find("bios_grub") != std::string::npos) {
            partitionDetails += " - bios_grub flag detected! --> USB drive may contain bootable content!\n";
            blackListedFileSystemOrFlag = true;
        }
        if (flags[i].find("hidden") != std::string::npos) {
            partitionDetails += " - hidden flag detected! --> USB drive tries to hide a partition!\n";
            blackListedFileSystemOrFlag = true;
        }
        if (flags[i].find("legacy_boot") != std::string::npos) {
            partitionDetails += " - legacy_boot flag detected! --> USB drive may contain bootable content!\n";
            blackListedFileSystemOrFlag = true;
        }
    }

    if (!blackListedFileSystemOrFlag) {
        partitionDetails += " - No black listed file system or flags detected.";
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

    fileExtensionDetails += " - " + std::to_string(fileCounter) + " files detected.\n";
    fileExtensionDetails += " - Black listed file extensions: ";
    for (const auto& item : Settings().fileExtensionBlacklist) {
        fileExtensionDetails += item + ", ";
    }
    fileExtensionDetails.pop_back();
    fileExtensionDetails.push_back('\n');

    if (blackListedFiles.empty()) {
        fileExtensionDetails += " - No black listed files detected.\n";
    } else {
        fileExtensionDetails += " - Following files are black listed:\n";
        for (const auto & blackListedFile : blackListedFiles) {
            fileExtensionDetails += " -- " + blackListedFile + "\n";
        }
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

    virusDetails += " - " + std::to_string(fileCounter) + " files were scanned.\n";
    virusDetails += " - " + std::to_string(signature) + " signature/s were loaded.\n";
    virusDetails += " - " + std::to_string(scannedData) + " MB data were scanned.\n";
    virusDetails += " - " + std::to_string(infectedFilesCounter) + " files infected.\n";

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
