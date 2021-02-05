#include "PrintingResultService.hpp"

PrintingResultService::PrintingResultService() = default;

std::string PrintingResultService::getCheckSystemRequirementResultForGui() {
    return " Fixing system requirements failed. \n Analysis process will be aborted. \n";
}

void PrintingResultService::printCheckSystemRequirementResultForTerminal() {
    clearScreen();
    std::cout << std::endl << getCheckSystemRequirementResultForGui() << std::endl;
    PrintingService::printWarningSignForTerminal();
    PrintingService::printConfirmForTerminal();
}

std::string PrintingResultService::getUsbPreCheckResultForGui() {
    return " Pre Check Failed! \n Please leave device disconnected until start.\n";
}

void PrintingResultService::printUsbPreCheckResultForTerminal() {
    clearScreen();
    std::cout << std::endl << getUsbPreCheckResultForGui() << std::endl;
    PrintingService::printWarningSignForTerminal();
    PrintingService::printConfirmForTerminal();
}

std::string PrintingResultService::getPeripheryAnalysisResultForGui(int result) {
    std::string resultText;

    switch (result) {
        case 1:
            resultText += " No device detected!";
            break;
        case 2:
            resultText += " Critical interface detected!";
            break;
        case 3:
            resultText += " Multiple problems detected!";
            break;
        default:
            resultText += " Unknown problem.";
    }

    resultText += "\n\n" + getContactAdminText();

    return resultText;
}

void PrintingResultService::printPeripheryAnalysisResultForTerminal(int result) {
    std::cout << std::endl << " Periphery Analysis Failed! \n" << getPeripheryAnalysisResultForGui(result) << std::endl;
    PrintingService::printWarningSignForTerminal();
}

std::string PrintingResultService::getPartitionAnalysisResultForGui(int result) {
    std::string resultText;

    switch (result) {
        case 1:
            resultText += " Authorizing failed!";
            break;
        case 2:
            resultText += " Binding failed!";
            break;
        case 3:
            resultText += " Could not read partition!";
            break;
        case 4:
            resultText += " No or multiple partitions detected!";
            break;
        case 5:
            resultText += " Critical partition detected!";
            break;
        default:
            resultText += " Unknown problem.";
    }

    resultText += "\n\n" + getContactAdminText();

    return resultText;
}

void PrintingResultService::printPartitionAnalysisResultForTerminal(int result) {
    std::cout << std::endl << " Partition Analysis Failed! \n" << getPartitionAnalysisResultForGui(result) << std::endl;
    PrintingService::printWarningSignForTerminal();
}

std::string PrintingResultService::getFileExtensionAnalysisResultForGui(int result) {
    std::string resultText;

    switch (result) {
        case 1:
            resultText += " Critical file/s detected!";
            break;
        case 2:
            resultText += " Unmounting went wrong!";
            break;
        case 3:
            resultText += " Mounting went wrong!";
            break;
        default:
            resultText += " Unknown problem.";
    }

    resultText += "\n\n" + getContactAdminText();

    return resultText;
}

void PrintingResultService::printFileExtensionAnalysisResultForTerminal(int result) {
    std::cout << std::endl << " File Extension Analysis Failed! \n" << getFileExtensionAnalysisResultForGui(result) << std::endl;
    PrintingService::printWarningSignForTerminal();
}

std::string PrintingResultService::getVirusAnalysisResultForGui(int result) {
    std::string resultText;

    switch (result) {
        case 1:
            resultText += " Initialisation of ClamAV library went wrong!";
            break;
        case 2:
            resultText += " Creating ClamAV engine went wrong!";
            break;
        case 3:
            resultText += " Loading database went wrong!";
            break;
        case 4:
            resultText += " Reloading database went wrong!";
            break;
        case 5:
            resultText += " Compiling ClamAV engine went wrong!";
            break;
        case 6:
            resultText += " Virus detected!";
            break;
        case 7:
            resultText += " Unspecific scan result!";
            break;
        case 8:
            resultText += " Could not access USB device or file!";
            break;
        default:
            return " Unknown problem.";
    }

    resultText += "\n\n" + getContactAdminText();

    return resultText;
}

void PrintingResultService::printVirusAnalysisResultForTerminal(int result) {
    std::cout << std::endl << " Virus Analysis Failed! \n" << getVirusAnalysisResultForGui(result) << std::endl;
    PrintingService::printWarningSignForTerminal();
}

void PrintingResultService::clearScreen() {
    // clear terminal with ANSI escape codes
    std::cout << "\033[2J\033[1;1H";
}

std::string PrintingResultService::getContactAdminText() {
    return " Please contact your IT administrator.";
}

PrintingResultService::~PrintingResultService() = default;
