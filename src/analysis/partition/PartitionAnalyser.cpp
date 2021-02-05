#include "PartitionAnalyser.hpp"

PartitionAnalyser::PartitionAnalyser(Device* pDevice) {
	device = pDevice;
}

int PartitionAnalyser::analysePartition(std::string *partitions, std::string *fileSystems) {
	if (!device->authorizeAll()) {
	    return 1;
	}

	// bind usb-storage driver manually to the first and only interface of the device
	if (!device->bindUsbStorageDriverToInterface(0)) {
		return 2;
	}

	// read partition information
    partitionCounter = getPartitions(partitions, fileSystems);

    if (partitionCounter == -1) {
        return 3;
    } else {
        return partitionAnalysis(fileSystems);
    }
}

std::string* PartitionAnalyser::getFlags() {
    return flags;
}

int PartitionAnalyser::getPartitionCounter() {
    return partitionCounter;
}

int PartitionAnalyser::getPartitions(std::string* partitions, std::string* filesystems) {
    FILE* fp = popen("parted -s -l -m 2>&1", "r");

    if (!fp) {
        return -1;
    }

    bool foundDevice = false;
    char* line = nullptr;
    char deviceName = ' ';
    int numOfPartition = 0;
    size_t len = 0;

    while (getline(&line, &len, fp) != -1) {
        std::string str(line);
        std::string substr("/dev/sd");

        // looking for the line /dev/sd starting on index 0
        // if line contains /dev/sd than skip else read partition
        if (str.find(substr) == 0) {
            foundDevice = true;
            deviceName = line[7];
        } else if (line[0] == '\n') {
            foundDevice = false;
        } else if (foundDevice) {
            int indexOfParameter = 0;
            std::string parameter;
            partitions[numOfPartition] = substr + deviceName + line[0];

            for (int i = 0; line[i] != '\n'; i++) {
                if (line[i] == ';') {
                    numOfPartition++;
                    break;
                } else if (line[i] != ':') {
                    parameter += line[i];
                } else {
                    if (indexOfParameter == 4) {
                        filesystems[numOfPartition] = parameter;
                    } else if (indexOfParameter == 6) {
                        flags[numOfPartition] = parameter;
                    }

                    indexOfParameter++;
                    parameter = "";
                }
            }
        }
    }
    pclose(fp);

    return numOfPartition;
}

int PartitionAnalyser::partitionAnalysis(std::string* fileSystems) {
	if (partitionCounter != 1) {
        return 4;
    } else {
		for (int i = 0; i < partitionCounter; i++) {
			if (fileSystems[i] == "efi" || fileSystems[i] == "ext2" || fileSystems[i] == "ext3" || fileSystems[i] == "ext4"
			        || flags[i].find("boot") != std::string::npos || flags[i].find("bios_grub") != std::string::npos
			        || flags[i].find("hidden") != std::string::npos || flags[i].find("legacy_boot") != std::string::npos) {
                return 5;
            }
		}
		return 0;
	}
}

PartitionAnalyser::~PartitionAnalyser() = default;
