#include "DuckyDetectorApplicationTerminal.hpp"

Scanner scanner(false);

DuckyDetectorApplicationTerminal::DuckyDetectorApplicationTerminal() = default;

int DuckyDetectorApplicationTerminal::run() {
    while (PrintingService::printStartOfScanProcessForTerminal()) {
        startScanningProcess();
        PrintingService::printResetForTerminal();
    }

    // Releasing heap memory and exiting
    PrintingService::printUnblockingPortsForTerminal();
    PrintingService::printByeForTerminal();
    scanner.freeMemory();
    return 0;
}

void DuckyDetectorApplicationTerminal::startScanningProcess() {
    if (!scanner.startCheckSystemRequirements()) {
        PrintingResultService::printCheckSystemRequirementResultForTerminal();
        showPreparationDetails();
        scanner.freeMemory();
        return;
    }

    if (!scanner.startUsbPreCheck()) {
        PrintingResultService::printUsbPreCheckResultForTerminal();
        return;
    }

    result = scanner.startPeripheryAnalysis(true);
    if (result != 0) {
        PrintingResultService::printPeripheryAnalysisResultForTerminal(result);
        showUsbPeripheryAnalysisDetails();
        return;
    }

    result = scanner.startPeripheryAnalysis(false);
    if (result != 0) {
        PrintingResultService::printPeripheryAnalysisResultForTerminal(result);
        showUsbPeripheryAnalysisDetails();
        return;
    } else {
        int continueAnalysis;

        while ((continueAnalysis = PrintingService::printContinueWithNextStageForTerminal("Periphery", "partition")) == 1) {
            showUsbPeripheryAnalysisDetails(true);
        }

        if (continueAnalysis == 0) {
            startPartitionAnalysis();
        } else {
            PrintingService::printAbortedByUserTextForTerminal();
        }
    }
}

void DuckyDetectorApplicationTerminal::startPartitionAnalysis() {
    PrintingService::printCurrentProcessForTerminal("partition/s");

    result = scanner.startPartitionAnalysis();
    if (result != 0) {
        PrintingResultService::printPartitionAnalysisResultForTerminal(result);
        showPartitionAnalysisDetails();
        return;
    } else {
        int continueAnalysis;

        while ((continueAnalysis = PrintingService::printContinueWithNextStageForTerminal("Partition", "file/s")) == 1) {
            showPartitionAnalysisDetails(true);
        }

        if (continueAnalysis == 0) {
            startFileExtensionAnalysis();
        } else {
            PrintingService::printAbortedByUserTextForTerminal();
        }
    }
}

void DuckyDetectorApplicationTerminal::startFileExtensionAnalysis() {
    PrintingService::printCurrentProcessForTerminal("file/s");

    result = scanner.startFileExtensionAnalysis();
    if (result != 0) {
        PrintingResultService::printFileExtensionAnalysisResultForTerminal(result);
        showFileExtensionAnalysisDetails();
        return;
    } else {
        int continueAnalysis;

        while ((continueAnalysis = PrintingService::printContinueWithNextStageForTerminal("File Extension", "viruses")) == 1) {
            showFileExtensionAnalysisDetails(true);
        }

        if (continueAnalysis == 0) {
            startMalwareAnalysis();
        } else {
            PrintingService::printAbortedByUserTextForTerminal();
        }
    }
}

void DuckyDetectorApplicationTerminal::startMalwareAnalysis() {
    PrintingService::printCurrentProcessForTerminal("viruses");

    result = scanner.startMalwareAnalysis();
    if (result != 0) {
        PrintingResultService::printVirusAnalysisResultForTerminal(result);
    } else {
        PrintingService::printProcessWentGoodTextForTerminal("Virus");
    }

    showMalwareAnalysisDetails();
}

void DuckyDetectorApplicationTerminal::showPreparationDetails() {
    if (PrintingDetailsService::printDetailsQuestion()) {
        PrintingDetailsService::printPreparationDetailsForTerminal(scanner.getPreparationHelperDetails());
    }
}

void DuckyDetectorApplicationTerminal::showUsbPeripheryAnalysisDetails(bool showDetailsWithoutAsking) {
    if (showDetailsWithoutAsking) {
        PrintingDetailsService::printUsbPeripheryAnalysisDetailsForTerminal(scanner.getUsbPeripheryAnalyserDetails(),
            scanner.getDevice());
    } else {
        if (PrintingDetailsService::printDetailsQuestion()) {
            PrintingDetailsService::printUsbPeripheryAnalysisDetailsForTerminal(scanner.getUsbPeripheryAnalyserDetails(),
                    scanner.getDevice());
        }
    }
}

void DuckyDetectorApplicationTerminal::showPartitionAnalysisDetails(bool showDetailsWithoutAsking) {
    if (showDetailsWithoutAsking) {
        PrintingDetailsService::printPartitionAnalysisDetailsForTerminal(scanner.getPartitionAnalyserPartitionCounterDetails(),
            scanner.getFilesystems(), scanner.getPartitionAnalyserFlagsDetails());
    } else {
        if (PrintingDetailsService::printDetailsQuestion()) {
            PrintingDetailsService::printPartitionAnalysisDetailsForTerminal(scanner.getPartitionAnalyserPartitionCounterDetails(),
                scanner.getFilesystems(), scanner.getPartitionAnalyserFlagsDetails());
        }
    }
}

void DuckyDetectorApplicationTerminal::showFileExtensionAnalysisDetails(bool showDetailsWithoutAsking) {
    if (showDetailsWithoutAsking) {
        PrintingDetailsService::printFileExtensionAnalysisDetailsForTerminal(scanner.getFileExtensionAnalyserFileCounterDetails(),
            scanner.getFileExtensionAnalyserBlackListedFilesDetails());
    } else {
        if (PrintingDetailsService::printDetailsQuestion()) {
            PrintingDetailsService::printFileExtensionAnalysisDetailsForTerminal(scanner.getFileExtensionAnalyserFileCounterDetails(),
                scanner.getFileExtensionAnalyserBlackListedFilesDetails());
        }
    }
}

void DuckyDetectorApplicationTerminal::showMalwareAnalysisDetails() {
    if (PrintingDetailsService::printDetailsQuestion()) {
        PrintingDetailsService::printVirusAnalysisDetailsForTerminal(scanner.getVirusAnalyserFileCounterDetails(),
            scanner.getVirusAnalyserNumberOfSignatureDetails(), scanner.getVirusAnalyserDataScannedInMbDetails(),
            scanner.getVirusAnalyserInfectedFilesCounterDetails(), scanner.getVirusAnalyserFoundVirusNamesDetails(),
            scanner.getVirusAnalyserInfectedFilesDetails(), scanner.getVirusAnalyserEngineStatusDetails());
    }
}

DuckyDetectorApplicationTerminal::~DuckyDetectorApplicationTerminal() = default;
