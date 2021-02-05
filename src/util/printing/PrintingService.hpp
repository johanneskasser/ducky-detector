#ifndef DUCKYDETECTOR_PRINTINGSERVICE_HPP
#define DUCKYDETECTOR_PRINTINGSERVICE_HPP

#include <iostream>
#include <unistd.h>

class PrintingService {
    public:
        PrintingService();
        virtual ~PrintingService();

        // Specific for Gui
        static std::string getLogoForGui();
        static std::string getByeTextForGui();

        // Specific for Terminal
        static void printLogoForTerminal();
        static void printCurrentProcessForTerminal(const std::string& analysisName);
        static void printWarningSignForTerminal();
        static void printAbortedByUserTextForTerminal();
        static void printOKForTerminal();
        static void printConfirmForTerminal();
        static void printByeForTerminal();

        // For Both
        static int checkInput(bool printWithoutDetails = true);
        static std::string getStartOfScanProcessTextForGui();
        static bool printStartOfScanProcessForTerminal();
        static std::string getContinueWithNextStageForGui(const std::string& analysisName);
        static int printContinueWithNextStageForTerminal(const std::string& currentAnalysisName, const std::string& nextAnalysisName);
        static std::string getProcessWentGoodTextForGui();
        static void printProcessWentGoodTextForTerminal(const std::string& currentAnalysisName);
        static std::string getResetTextForGui();
        static void printResetForTerminal();
        static std::string getUnblockingPortsTextForGui();
        static void printUnblockingPortsForTerminal();

    private:
        static void clearScreen();
};

#endif //DUCKYDETECTOR_PRINTINGSERVICE_HPP
