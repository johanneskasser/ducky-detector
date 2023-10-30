#include "PrintingService.hpp"

PrintingService::PrintingService() = default;

// Specific for Gui
std::string PrintingService::getLogoForGui() {
    std::string logo;

    logo = "                                       _ \n";
    logo += "                                ___( . )<     Ducky Detector \n";
    logo += "                                \\____) \n\n";
    logo += "                                Welcome to the mighty \n";
    logo += "                                        Ducky Detector! \n\n";

    return logo;
}

//std::string PrintingService::getTermsAndCondition() {
//    std::string termsAndCondition;
//
//    termsAndCondition += " Please read carefully!\n\n";
//    termsAndCondition += " Before we start, I need to make some security arrange- \n";
//    termsAndCondition += " ments, which means that from now on no external usb \n";
//    termsAndCondition += " devices will be working anymore.\n";
//    termsAndCondition += " NO keyboards and NO mice! \n";
//    termsAndCondition += " Internal components like wifi and RJ45 Interfaces are not \n";
//    termsAndCondition += " affected. Continue only if you are using a touch display or\n";
//    termsAndCondition += " any remote connection to this device like SSH or VNC in \n";
//    termsAndCondition += " case something goes wrong. Ports should be accessible \n";
//    termsAndCondition += " after exiting the program.\n\n";
//    termsAndCondition += " I read about the consequences and want to continue:";
//
//    return termsAndCondition;
//}

std::string PrintingService::getByeTextForGui() {
    std::string byeText;

    byeText +="  _                   \n";
    byeText +=" | |__  _   _  ___    \n";
    byeText +=" |  _ \\| | | |/ _ \\ \n";
    byeText +=" | |_) | |_| |  __/   \n";
    byeText +=" |_.__/ \\__, |\\___| \n";
    byeText +="        |___/         \n";

    return byeText;
}

// Specific for Terminal
void PrintingService::printLogoForTerminal() {
    std::string logo;

    logo += "                              _ \n";
    logo += "                           __(.)<     Ducky Detector \n";
    logo += "                          \\___) \n\n";
    logo += "                 Welcome to the mighty Ducky Detector!\n";

    std::cout << std::endl << logo << std::endl;
}

void PrintingService::printCurrentProcessForTerminal(const std::string& analysisName) {
    clearScreen();
    std::cout << " Analysing " + analysisName << std::endl;
}

void PrintingService::printWarningSignForTerminal() {
    std::string warningSign;

    warningSign += "  _   _   _  \n";
    warningSign += " | | | | | | \n";
    warningSign += " | | | | | | \n";
    warningSign += " |_| |_| |_| \n";
    warningSign += "  _   _   _  \n";
    warningSign += " (_) (_) (_) \n";

    std::cout << warningSign << std::endl;
}

void PrintingService::printAbortedByUserTextForTerminal() {
    std::cout << std::endl << " Analysis process was aborted by user." << std::endl;
}

void PrintingService::printOKForTerminal() {
    std::string okText;

    okText += "    ___  _  __    \n";
    okText += "   / _ \\| |/ /   \n";
    okText += "  | | | | ' /     \n";
    okText += "  | |_| | . \\    \n";
    okText += "   \\___/|_|\\_\\ \n";

    std::cout << std::endl << okText << std::endl;
}

void PrintingService::printConfirmForTerminal() {
    char input;
    std::cout << std::endl << " [O]K:" << std::endl << " ";
    std::cin >> input;
}

void PrintingService::printByeForTerminal() {
    std::string byeText;

    byeText += "  _                   \n";
    byeText += " | |__  _   _  ___    \n";
    byeText += " |  _ \\| | | |/ _ \\ \n";
    byeText += " | |_) | |_| |  __/   \n";
    byeText += " |_.__/ \\__, |\\___| \n";
    byeText += "        |___/         \n";

    std::cout << std::endl << byeText << std::endl;
}

// For Both
std::string PrintingService::getStartOfScanProcessTextForGui() {
    return " Do you want to scan a drive?";
}

bool PrintingService::printStartOfScanProcessForTerminal() {
    clearScreen();
    printLogoForTerminal();
    std::cout << std::endl << getStartOfScanProcessTextForGui() << std::endl;
    return checkInput() != -1;
}

std::string PrintingService::getContinueWithNextStageForGui(const std::string& analysisName) {
    std::string continueText;

    continueText += " So far the analysis process went well.\n\n";
    continueText += " Press 'OK' to continue with " + analysisName + " analysis process.\n";
    continueText += " Press 'Details' to view the details.\n";
    continueText += " Press 'Cancel' to abort analysis process.";

    return continueText;
}

int PrintingService::printContinueWithNextStageForTerminal(const std::string& currentAnalysisName, const std::string& nextAnalysisName) {
    clearScreen();
    std::cout << " Stage " + currentAnalysisName + " Analysis\n";
    printOKForTerminal();
    std::cout << std::endl << getContinueWithNextStageForGui(nextAnalysisName) << std::endl;
    return checkInput(false);
}

std::string PrintingService::getProcessWentGoodTextForGui() {
    return " Whole analysis process went good. No Threats detected.";
}

void PrintingService::printProcessWentGoodTextForTerminal(const std::string& currentAnalysisName) {
    clearScreen();
    std::cout << " Stage " + currentAnalysisName + " Analysis\n";
    printOKForTerminal();
    std::cout << std::endl << getProcessWentGoodTextForGui() << std::endl;
}

std::string PrintingService::getResetTextForGui() {
    return "\n Remove device and press OK.";
}

void PrintingService::printResetForTerminal() {
    clearScreen();
    std::cout << getResetTextForGui() << std::endl;
    printConfirmForTerminal();
}

std::string PrintingService::getUnblockingPortsTextForGui() {
    std::string text = " Unblocking USB Ports... \n";
    text += " Deleting engine and freeing spaces.";
    return text;
}

void PrintingService::printUnblockingPortsForTerminal() {
    clearScreen();
    std::cout << getUnblockingPortsTextForGui() << std::endl;
}

int PrintingService::checkInput(bool printWithoutDetails) {
    char input;
    if (printWithoutDetails) {
        std::cout << std::endl << " [Y]es or [N]o" << std::endl << " ";
        std::cin >> input;
    } else {
        std::cout << std::endl << " [O]K or [D]etails or [C]ancel" << std::endl << " ";
        std::cin >> input;
    }

    switch (input) {
        case 'O':
        case 'o':
        case 'Y':
        case 'y':
            return 0;
        case 'D':
        case 'd':
            return 1;
        default:
            return -1;
    }
}

void PrintingService::clearScreen() {
    // clear terminal with ANSI escape codes
    std::cout << "\033[2J\033[1;1H";
}

PrintingService::~PrintingService() = default;
