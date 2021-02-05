#ifndef DUCKYDETECTOR_SCANNER_HPP // prevents multiple compiles
#define DUCKYDETECTOR_SCANNER_HPP

#include <map>
#include "preparation/PreparationHelper.hpp"
#include "partition/PartitionAnalyser.hpp"
#include "usbPeriphery/UsbPeripheryAnalyser.hpp"
#include "fileExtension/FileExtensionAnalyser.hpp"
#include "virus/MalwareAnalyserWithClamAv.hpp"
#include "../util/Device.hpp"

class Scanner {
    public:
        Scanner(bool pRunWithGui = true);
        virtual ~Scanner();

        bool startCheckSystemRequirements();
        bool startUsbPreCheck();
        int startPeripheryAnalysis(bool initial);
        int startPartitionAnalysis();
        int startFileExtensionAnalysis();
        int startMalwareAnalysis();
        /**
         * Releasing heap memory and exiting
         */
        void freeMemory();
        Device& getDevice();
        std::map<std::string, bool> getPreparationHelperDetails();
        std::map<std::string, bool> getUsbPeripheryAnalyserDetails();
        const std::string* getFilesystems() const;
        std::string* getPartitionAnalyserFlagsDetails();
        int getPartitionAnalyserPartitionCounterDetails();
        std::set<std::string> getFileExtensionAnalyserBlackListedFilesDetails();
        int getFileExtensionAnalyserFileCounterDetails();
        long double getVirusAnalyserDataScannedInMbDetails();
        std::map<std::string, bool> getVirusAnalyserEngineStatusDetails();
        int getVirusAnalyserFileCounterDetails();
        std::set<std::string> getVirusAnalyserFoundVirusNamesDetails();
        std::set<std::string> getVirusAnalyserInfectedFilesDetails();
        int getVirusAnalyserInfectedFilesCounterDetails();
        unsigned int getVirusAnalyserNumberOfSignatureDetails();

    private:
        bool runWithGui;
        std::string partitions[20];
        std::string filesystems[20];
        // this is the target device going to be scanned. ONLY ONE ALLOWED!
        Device device;
        Settings settings;
        PreparationHelper preparationHelper;
        UsbPeripheryAnalyser usbPeripheryAnalyser;
        PartitionAnalyser partitionAnalyser;
        FileExtensionAnalyser fileExtensionAnalyser;
        MalwareAnalyserWithClamAv virusAnalyser;
};

#endif //DUCKYDETECTOR_SCANNER_HPP