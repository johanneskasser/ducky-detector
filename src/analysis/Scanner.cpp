#include "Scanner.hpp"

Scanner::Scanner(bool pRunWithGui) : preparationHelper(&device), usbPeripheryAnalyser(&device), partitionAnalyser(&device),
    fileExtensionAnalyser(&device), virusAnalyser(&device) {
    runWithGui = pRunWithGui;
}

bool Scanner::startCheckSystemRequirements() {
    return preparationHelper.checkSystemRequirements();
}

bool Scanner::startUsbPreCheck() {
    return preparationHelper.usbPreCheck();
}

int Scanner::startPeripheryAnalysis(bool initial) {
    return usbPeripheryAnalyser.peripheryAnalysis(initial, runWithGui);
}

int Scanner::startPartitionAnalysis() {
    return partitionAnalyser.analysePartition(partitions, filesystems);
}

int Scanner::startFileExtensionAnalysis() {
    return fileExtensionAnalyser.analyseFileExtensions(partitions, filesystems);
}

int Scanner::startMalwareAnalysis() {
    return virusAnalyser.analyseForMalware(partitions, filesystems);
}

void Scanner::freeMemory() {
    preparationHelper.restoreSystemState();
    virusAnalyser.deleteEngine();
}

std::map<std::string, bool> Scanner::getPreparationHelperDetails() {
    return preparationHelper.getPreparationHelperDetails();
}

std::map<std::string, bool> Scanner::getUsbPeripheryAnalyserDetails() {
    return usbPeripheryAnalyser.getUsbPeripheryAnalyserDetails();
}

std::string* Scanner::getPartitionAnalyserFlagsDetails() {
    return partitionAnalyser.getFlags();
}

int Scanner::getPartitionAnalyserPartitionCounterDetails() {
    return partitionAnalyser.getPartitionCounter();
}

std::set<std::string> Scanner::getFileExtensionAnalyserBlackListedFilesDetails() {
    return fileExtensionAnalyser.getBlackListedFiles();
}

int Scanner::getFileExtensionAnalyserFileCounterDetails() {
    return fileExtensionAnalyser.getFileCounter();
}

long double Scanner::getVirusAnalyserDataScannedInMbDetails() {
    return virusAnalyser.getDataScannedInMb();
}

std::map<std::string, bool> Scanner::getVirusAnalyserEngineStatusDetails() {
    return virusAnalyser.getEngineStatus();
}

int Scanner::getVirusAnalyserFileCounterDetails() {
    return virusAnalyser.getFileCounter();
}

std::set<std::string> Scanner::getVirusAnalyserFoundVirusNamesDetails(){
    return virusAnalyser.getFoundVirusNames();
}

std::set<std::string> Scanner::getVirusAnalyserInfectedFilesDetails() {
    return virusAnalyser.getInfectedFiles();
}

int Scanner::getVirusAnalyserInfectedFilesCounterDetails() {
    return virusAnalyser.getInfectedFilesCounter();
}

unsigned int Scanner::getVirusAnalyserNumberOfSignatureDetails() {
    return virusAnalyser.getNumberOfSignature();
}

const std::string* Scanner::getFilesystems() const {
    return filesystems;
}

Device& Scanner::getDevice() {
    return device;
}

Scanner::~Scanner() = default;
