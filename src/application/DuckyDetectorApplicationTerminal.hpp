#ifndef DUCKYDETECTOR_DUCKYDETECTORAPPLICATIONTERMINAL_HPP
#define DUCKYDETECTOR_DUCKYDETECTORAPPLICATIONTERMINAL_HPP

#include "../analysis/Scanner.hpp"
#include "../util/printing/PrintingService.hpp"
#include "../util/printing/PrintingResultService.hpp"
#include "../util/printing/PrintingDetailsService.hpp"

class DuckyDetectorApplicationTerminal {
    public:
        DuckyDetectorApplicationTerminal();
        virtual ~DuckyDetectorApplicationTerminal();

        int run();

    private:
        int result = 0;

        void startScanningProcess();
        void startPartitionAnalysis();
        void startFileExtensionAnalysis();
        void startMalwareAnalysis();
        static void showPreparationDetails();
        static void showUsbPeripheryAnalysisDetails(bool showDetailsWithoutAsking = false);
        static void showPartitionAnalysisDetails(bool showDetailsWithoutAsking = false);
        static void showFileExtensionAnalysisDetails(bool showDetailsWithoutAsking = false);
        static void showMalwareAnalysisDetails();
};

#endif //DUCKYDETECTOR_DUCKYDETECTORAPPLICATIONTERMINAL_HPP
