#ifndef DUCKYDETECTOR_PRINTINGRESULTSERVICE_HPP
#define DUCKYDETECTOR_PRINTINGRESULTSERVICE_HPP

#include <iostream>
#include "PrintingService.hpp"

class PrintingResultService {
    public:
        PrintingResultService();
        virtual ~PrintingResultService();

        static std::string getCheckSystemRequirementResultForGui();
        static void printCheckSystemRequirementResultForTerminal();
        static std::string getUsbPreCheckResultForGui();
        static void printUsbPreCheckResultForTerminal();
        /**
        * Prints the result of the periphery analysis
        *
        * @param result
        * 0 - Everything went good
        * 1 - No devices were found!
        * 2 - Critical interface found!
        * 3 - Multiple problems found!
        */
        static std::string getPeripheryAnalysisResultForGui(int result);
        static void printPeripheryAnalysisResultForTerminal(int result);
        /**
         * Prints the result of the partition analysis
         *
         * @param result
         * 0 - OK, if exactly one partition was found.
         * 1 - Authorizing failed!
         * 2 - Binding failed!
         * 3 - Could not read partition!
         * 4 - No or multiple partitions found!
         * 5 - Critical partition found!
         */
        static std::string getPartitionAnalysisResultForGui(int result);
        static void printPartitionAnalysisResultForTerminal(int result);
        /**
         * Prints the result of the file extension analysis
         *
         * @param result
         * 0 - OK, if no critical file/s were found
         * 1 - Critical file/s found!
         * 2 - Unmounting went wrong!
         * 3 - Mounting went wrong!
         */
        static std::string getFileExtensionAnalysisResultForGui(int result);
        static void printFileExtensionAnalysisResultForTerminal(int result);
        /**
         * Prints the result of the virus analysis
         *
         * @param result
         * 0 - OK, analysis went fine
         * 1 - Initialisation of ClamAV library went wrong!
         * 2 - Creating ClamAV engine went wrong!
         * 3 - Loading database went wrong!
         * 4 - Reloading database went wrong!
         * 5 - Compiling ClamAV engine went wrong!
         * 6 - Virus detected!
         * 7 - Unspecific scan result!
         * 8 - Could not access USB device or file!
         */
        static std::string getVirusAnalysisResultForGui(int result);
        static void printVirusAnalysisResultForTerminal(int result);

    private:
        static void clearScreen();
        static std::string getContactAdminText();
};

#endif //DUCKYDETECTOR_PRINTINGRESULTSERVICE_HPP
