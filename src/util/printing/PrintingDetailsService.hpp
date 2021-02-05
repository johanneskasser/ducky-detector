#ifndef DUCKYDETECTOR_PRINTINGDETAILSSERVICE_HPP
#define DUCKYDETECTOR_PRINTINGDETAILSSERVICE_HPP

#include <iostream>
#include <map>
#include <set>
#include "../Device.hpp"
#include "PrintingService.hpp"

class PrintingDetailsService {
    public:
        PrintingDetailsService();
        virtual ~PrintingDetailsService();

        static bool printDetailsQuestion();
        static std::string printDeviceInfos(Device& device);
        static std::string getPreparationDetailsForGui(const std::map<std::string, bool>& preparationAnalysisDetails);
        static void printPreparationDetailsForTerminal(const std::map<std::string, bool>& preparationAnalysisDetails);
        static std::string getPeripheryAnalysisDetailsForGui(const std::map<std::string, bool>& peripheryAnalysisDetails,
            Device& device);
        static void printUsbPeripheryAnalysisDetailsForTerminal(const std::map<std::string, bool>& peripheryAnalysisDetails,
            Device& device);
        static std::string getPartitionAnalysisDetailsForGui(int partitionCounter, const std::string* fileSystems, std::string* flags);
        static void printPartitionAnalysisDetailsForTerminal(int partitionCounter, const std::string* fileSystems, std::string* flags);
        static std::string getFileExtensionAnalysisDetailsForGui(int fileCounter, const std::set<std::string>& blackListedFiles);
        static void printFileExtensionAnalysisDetailsForTerminal(int fileCounter, const std::set<std::string>& blackListedFiles);
        static std::string getVirusAnalysisDetailsForGui(int fileCounter, unsigned int signature, long double scannedData,
            int infectedFilesCounter, const std::set<std::string>& virusNames, const std::set<std::string>& infectedFiles,
            std::map<std::string, bool> engineStatus);
        static void printVirusAnalysisDetailsForTerminal(int fileCounter, unsigned int signature, long double scannedData,
            int infectedFilesCounter, std::set<std::string> virusNames, std::set<std::string> infectedFiles,
            std::map<std::string, bool> engineStatus);

    private:
        static void printDetailsSignForTerminal();
};

#endif //DUCKYDETECTOR_PRINTINGDETAILSSERVICE_HPP
