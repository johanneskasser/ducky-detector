#include "FileExtensionAnalyser.hpp"

FileExtensionAnalyser::FileExtensionAnalyser(Device* pDevice) {
    device = pDevice;
}

int FileExtensionAnalyser::analyseFileExtensions(std::string* partitions, std::string* filesystems) {
    fileCounter = 0;
    blackListedFiles.clear();

	if (device->handleUsbMounting(true, partitions[0], filesystems[0])) {
		fileHandler.fileDiscovery(settings.mountPoint, true, [this](const std::string &path) {
            fileExtensionBlacklistChecker(path);
            fileCounter++;
		});

		if (!device->handleUsbMounting(false, "", "")) {
			return 2;
		}

		if (blackListedFiles.empty()) {
		    return 0;
		} else {
		    return 1;
		}

    } else {
		return 3;
	}
}

std::set<std::string> FileExtensionAnalyser::getBlackListedFiles() {
    return blackListedFiles;
}

int FileExtensionAnalyser::getFileCounter() {
    return fileCounter;
}

void FileExtensionAnalyser::fileExtensionBlacklistChecker(const std::string& filename) {
	int sizeOfBlackListedFileExtensions = sizeof(settings.fileExtensionBlacklist)/sizeof(settings.fileExtensionBlacklist[0]);
    std::string fileExtension = getExtensionOfFile(filename);

	for (int i = 0; i < sizeOfBlackListedFileExtensions; i++) {
        if (fileExtension == settings.fileExtensionBlacklist[i]) {
            blackListedFiles.insert(filename);
        }
    }
}

std::string FileExtensionAnalyser::getExtensionOfFile(const std::string& filename){
    std::string result;
    std::size_t foundPosition = filename.rfind('.');

    if (foundPosition != std::string::npos) {
        result = filename.substr(foundPosition, filename.length());
    }

    return result;
}

FileExtensionAnalyser::~FileExtensionAnalyser() = default;
