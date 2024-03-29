#include "DuckyDetectorApplicationGui.hpp"

DuckyDetectorApplicationGui::DuckyDetectorApplicationGui() : Gtk::Application("fh.campuswien.duckyDetector",
                                                                              Gio::APPLICATION_FLAGS_NONE) {
}

Glib::RefPtr<DuckyDetectorApplicationGui> DuckyDetectorApplicationGui::create() {
    return Glib::RefPtr<DuckyDetectorApplicationGui>(new DuckyDetectorApplicationGui());
}

DuckyDetectorGui *DuckyDetectorApplicationGui::createApplicationWindow() {
    applicationWindow = new DuckyDetectorGui();
    add_window(*applicationWindow);
    applicationWindow->signal_hide().connect(sigc::bind<Gtk::Window *>(sigc::mem_fun(*this,
                                                                                     &DuckyDetectorApplicationGui::onHideWindow),
                                                                       applicationWindow));

    runApplication();

    //applicationWindow->endLoading();

    return applicationWindow;
}


void DuckyDetectorApplicationGui::on_activate() {
    applicationWindow = createApplicationWindow();
    applicationWindow->present();
    //applicationWindow->fullscreen();
}

void DuckyDetectorApplicationGui::runApplication() {
    resetWindow();
    applicationWindow->showProgressBar();
    applicationWindow->setText(PrintingService::getLogoForGui() + PrintingService::getStartOfScanProcessTextForGui());

    applicationWindow->endLoading();

    okButtonSignalConnection.disconnect();
    okButtonSignalConnection = applicationWindow->okButton->signal_clicked().connect(
            sigc::mem_fun(*this, &DuckyDetectorApplicationGui::runApplicationInterceptor));

    detailsButtonSignalConnection.disconnect();

    cancelButtonSignalConnection.disconnect();
    cancelButtonSignalConnection = applicationWindow->cancelButton->signal_clicked().connect(sigc::mem_fun(*this,
                                                                                                           &DuckyDetectorApplicationGui::onCancelButtonClicked));

}

void DuckyDetectorApplicationGui::runApplicationInterceptor() {
    if (applicationWindow->returnFastRunStatus()) {
        onFastAnalysis();
    } else {
        onStartScanProcessAndCheckSystemRequirements();
    }
}

void DuckyDetectorApplicationGui::onStartScanProcessAndCheckSystemRequirements() {
    applicationWindow->setModuleName("Start Process and Check System Requirements");

    detailsButtonSignalConnection = applicationWindow->detailsButton->signal_clicked().connect(sigc::mem_fun(*this,
                                                                                                             &DuckyDetectorApplicationGui::onPreparationDetails));

    cancelButtonSignalConnection.disconnect();
    cancelButtonSignalConnection = applicationWindow->cancelButton->signal_clicked().connect(sigc::mem_fun(*this,
                                                                                                           &DuckyDetectorApplicationGui::onReset));

    if (!scanner.startCheckSystemRequirements()) {
        applicationWindow->showError("Fixing System Requirements FAILED!");
        applicationWindow->setText(PrintingResultService::getCheckSystemRequirementResultForGui());
        onReset();
    } else {
        applicationWindow->setProgress(step);
        onUsbPreCheck();
    }
}

void DuckyDetectorApplicationGui::onUsbPreCheck() {
    applicationWindow->setModuleName("USB Pre-Check");

    detailsButtonSignalConnection.disconnect();
    detailsButtonSignalConnection = applicationWindow->detailsButton->signal_clicked().connect(sigc::mem_fun(*this,
                                                                                                             &DuckyDetectorApplicationGui::onUsbPreCheckDetails));

    if (!scanner.startUsbPreCheck()) {
        applicationWindow->showError("Pre Check FAILED!");
        applicationWindow->setText(PrintingResultService::getUsbPreCheckResultForGui());
        onReset();
    } else {
        applicationWindow->setProgress(step);
        onInitialPeripheryAnalysis();
    }
}

class PeripheryAnalysis {
public:
    int operator()() const {
        Scanner scanner;
        return scanner.startPeripheryAnalysis(true);
    }
};

void DuckyDetectorApplicationGui::onInitialPeripheryAnalysis() {
    applicationWindow->setModuleName("Initial Pheriphery Analysis");

    int result;
    PeripheryAnalysis peripheryAnalysis;

    detailsButtonSignalConnection.disconnect();
    detailsButtonSignalConnection = applicationWindow->detailsButton->signal_clicked().connect(sigc::mem_fun(*this,
                                                                                                             &DuckyDetectorApplicationGui::onPeripheryDetails));

    applicationWindow->showInfoDialog();
    result = applicationWindow->executeBackgroundTask(peripheryAnalysis);

    if (result != 0) {
        applicationWindow->showError("Periphery Analysis FAILED!");
        applicationWindow->setText(PrintingResultService::getPeripheryAnalysisResultForGui(result));
        onReset();
    } else {
        applicationWindow->setProgress(step);
        onSecondPeripheryAnalysis();
    }
}

class SecondPeripheryAnalysis {
public:
    int operator()() const {
        Scanner scanner;
        return scanner.startPeripheryAnalysis(false);
    }
};

void DuckyDetectorApplicationGui::onSecondPeripheryAnalysis() {
    applicationWindow->setModuleName("Second Pheriphery Analysis");

    int result;
    SecondPeripheryAnalysis secondPeripheryAnalysis;
    result = applicationWindow->executeBackgroundTask(secondPeripheryAnalysis);

    if (result != 0) {
        applicationWindow->showError("Periphery Analysis FAILED!");
        applicationWindow->setText(PrintingResultService::getPeripheryAnalysisResultForGui(result));
        onReset();
    } else {
        applicationWindow->setProgress(step);

        applicationWindow->showSuccess("Periphery Analysis SUCCESSFUL!");
        applicationWindow->setText(PrintingService::getContinueWithNextStageForGui("partition"));

        okButtonSignalConnection.disconnect();
        okButtonSignalConnection = applicationWindow->okButton->signal_clicked().connect(sigc::mem_fun(*this,
                                                                                                       &DuckyDetectorApplicationGui::onStartPartitionAnalysis));
    }
}

class PartitionAnalysis {
public:
    int operator()() const {
        Scanner scanner;
        return scanner.startPartitionAnalysis();
    }
};

void DuckyDetectorApplicationGui::onStartPartitionAnalysis() {
    applicationWindow->setModuleName("Partition Analysis");

    int result;
    PartitionAnalysis partitionAnalysis;

    detailsButtonSignalConnection.disconnect();
    detailsButtonSignalConnection = applicationWindow->detailsButton->signal_clicked().connect(sigc::mem_fun(*this,
                                                                                                             &DuckyDetectorApplicationGui::onPartitionDetails));

    result = applicationWindow->executeBackgroundTask(partitionAnalysis);

    if (result != 0) {
        applicationWindow->showError("Partition Analysis FAILED!");
        applicationWindow->setText(PrintingResultService::getPartitionAnalysisResultForGui(result));
        onReset();
    } else {
        applicationWindow->setProgress(step);
        applicationWindow->showSuccess("Partition Analysis SUCCESSFUL!");
        applicationWindow->setText(PrintingService::getContinueWithNextStageForGui("file extension"));

        okButtonSignalConnection.disconnect();
        okButtonSignalConnection = applicationWindow->okButton->signal_clicked().connect(sigc::mem_fun(*this,
                                                                                                       &DuckyDetectorApplicationGui::onStartFileExtensionAnalysis));
    }
}

class FileExtAnalysis {
public:
    int operator()() const {
        Scanner scanner;
        return scanner.startFileExtensionAnalysis();
    }
};

void DuckyDetectorApplicationGui::onStartFileExtensionAnalysis() {
    applicationWindow->setModuleName("File Extension Analysis");

    int result;
    FileExtAnalysis fileExtAnalysis;

    detailsButtonSignalConnection.disconnect();
    detailsButtonSignalConnection = applicationWindow->detailsButton->signal_clicked().connect(sigc::mem_fun(*this,
                                                                                                             &DuckyDetectorApplicationGui::onFileExtensionDetails));

    //applicationWindow->startLoading();
    result = applicationWindow->executeBackgroundTask(fileExtAnalysis);

    //applicationWindow->endLoading();

    if (result != 0) {
        applicationWindow->showError("File Extension Analysis FAILED!");
        applicationWindow->setText(PrintingResultService::getFileExtensionAnalysisResultForGui(result));
        onReset();
    } else {
        applicationWindow->setProgress(step);
        applicationWindow->showSuccess("File Extension Analysis SUCCESSFUL!");
        applicationWindow->setText(PrintingService::getContinueWithNextStageForGui("malware"));

        okButtonSignalConnection.disconnect();
        okButtonSignalConnection = applicationWindow->okButton->signal_clicked().connect(sigc::mem_fun(*this,
                                                                                                       &DuckyDetectorApplicationGui::onStartMalwareAnalysis));
    }
}

class MalwareAnalysis {
public:
    int operator()() const {
        Scanner scanner;
        return scanner.startMalwareAnalysis();
    }
};

void DuckyDetectorApplicationGui::onStartMalwareAnalysis() {
    applicationWindow->setModuleName("Malware Analysis");

    int result;
    MalwareAnalysis malwareAnalysis;

    detailsButtonSignalConnection.disconnect();
    detailsButtonSignalConnection = applicationWindow->detailsButton->signal_clicked().connect(sigc::mem_fun(*this,
                                                                                                             &DuckyDetectorApplicationGui::onMalwareDetails));

    result = applicationWindow->executeBackgroundTask(malwareAnalysis);

    if (result != 0) {
        applicationWindow->showError("Virus Analysis FAILED!");
        applicationWindow->setText(PrintingResultService::getVirusAnalysisResultForGui(result));
        onReset();
    } else {
        applicationWindow->setProgress(step);
        applicationWindow->showSuccess("Virus Analysis SUCCESSFUL!");
        applicationWindow->setText(PrintingService::getProcessWentGoodTextForGui());

        okButtonSignalConnection.disconnect();
        okButtonSignalConnection = applicationWindow->okButton->signal_clicked().connect(sigc::mem_fun(*this,
                                                                                                       &DuckyDetectorApplicationGui::onReset));
    }
}

void DuckyDetectorApplicationGui::onReset() {
    applicationWindow->setTextForReset(PrintingService::getResetTextForGui());

    okButtonSignalConnection.disconnect();
    okButtonSignalConnection = applicationWindow->okButton->signal_clicked().connect(sigc::mem_fun(*this,
                                                                                                   &DuckyDetectorApplicationGui::runApplication));

    cancelButtonSignalConnection.disconnect();
    cancelButtonSignalConnection = applicationWindow->cancelButton->signal_clicked().connect(sigc::mem_fun(*this,
                                                                                                           &DuckyDetectorApplicationGui::runApplication));
}

void DuckyDetectorApplicationGui::onPreparationDetails() {
    applicationWindow->showDetailsDialog(
            PrintingDetailsService::getPreparationDetailsForGui(scanner.getPreparationHelperDetails()));
}

void DuckyDetectorApplicationGui::onUsbPreCheckDetails() {
    applicationWindow->showDetailsDialog(PrintingResultService::getUsbPreCheckResultForGui());
}

void DuckyDetectorApplicationGui::onPeripheryDetails() {
    applicationWindow->showDetailsDialog(
            PrintingDetailsService::getPeripheryAnalysisDetailsForGui(scanner.getUsbPeripheryAnalyserDetails(),
                                                                      scanner.getDevice()));
}

void DuckyDetectorApplicationGui::onPartitionDetails() {
    applicationWindow->showDetailsDialog(PrintingDetailsService::getPartitionAnalysisDetailsForGui(
            scanner.getPartitionAnalyserPartitionCounterDetails(),
            scanner.getFilesystems(), scanner.getPartitionAnalyserFlagsDetails()));
}

void DuckyDetectorApplicationGui::onFileExtensionDetails() {
    applicationWindow->showDetailsDialog(PrintingDetailsService::getFileExtensionAnalysisDetailsForGui(
            scanner.getFileExtensionAnalyserFileCounterDetails(),
            scanner.getFileExtensionAnalyserBlackListedFilesDetails()));
}

void DuckyDetectorApplicationGui::onMalwareDetails() {
    applicationWindow->showDetailsDialog(
            PrintingDetailsService::getVirusAnalysisDetailsForGui(scanner.getVirusAnalyserFileCounterDetails(),
                                                                  scanner.getVirusAnalyserNumberOfSignatureDetails(),
                                                                  scanner.getVirusAnalyserDataScannedInMbDetails(),
                                                                  scanner.getVirusAnalyserInfectedFilesCounterDetails(),
                                                                  scanner.getVirusAnalyserFoundVirusNamesDetails(),
                                                                  scanner.getVirusAnalyserInfectedFilesDetails(),
                                                                  scanner.getVirusAnalyserEngineStatusDetails()));
}

void DuckyDetectorApplicationGui::onCancelButtonClicked() {
    okButtonSignalConnection.disconnect();
    detailsButtonSignalConnection.disconnect();
    cancelButtonSignalConnection.disconnect();
    applicationWindow->setText(PrintingService::getUnblockingPortsTextForGui() + PrintingService::getByeTextForGui());
    scanner.freeMemory();
    applicationWindow->unset_application();
}

void DuckyDetectorApplicationGui::onHideWindow(Gtk::Window *window) {
    delete window;
}

DuckyDetectorApplicationGui::~DuckyDetectorApplicationGui() {
    delete applicationWindow;
}

void DuckyDetectorApplicationGui::resetWindow() {
    applicationWindow->setModuleName("Rubber Ducky Detector");
    applicationWindow->resetModuleNameBackground();
    //applicationWindow->hideInfoBar();
    applicationWindow->resetProgressBar();
}


void DuckyDetectorApplicationGui::onFastAnalysis() {
    applicationWindow->setModuleName("Fast Analysis");
    applicationWindow->startLoading();

    if (!scanner.startCheckSystemRequirements()) {
        applicationWindow->showError("Fixing System Requirements FAILED!");
        applicationWindow->setText(PrintingResultService::getCheckSystemRequirementResultForGui());
        onReset();
    } else {
        applicationWindow->setProgress(step);
        if (!scanner.startUsbPreCheck()) {
            applicationWindow->showError("Pre Check FAILED!");
            applicationWindow->setText(PrintingResultService::getUsbPreCheckResultForGui());
            onReset();
        } else {
            applicationWindow->setProgress(step);
            applicationWindow->showInfoDialog();
            int firstPeripheryAnalysisResult;
            firstPeripheryAnalysisResult = scanner.startPeripheryAnalysis(true);

            if (firstPeripheryAnalysisResult != 0) {
                applicationWindow->showError("Periphery Analysis FAILED!");
                applicationWindow->setText(
                        PrintingResultService::getPeripheryAnalysisResultForGui(firstPeripheryAnalysisResult));
                onReset();
            } else {
                applicationWindow->setProgress(step);
                int secondPeripheryAnalysis;
                secondPeripheryAnalysis = scanner.startPeripheryAnalysis(false);

                if (secondPeripheryAnalysis != 0) {
                    applicationWindow->showError("Periphery Analysis FAILED!");
                    applicationWindow->setText(
                            PrintingResultService::getPeripheryAnalysisResultForGui(secondPeripheryAnalysis));
                    onReset();
                } else {
                    applicationWindow->setProgress(step);
                    int partAnalysis;

                    partAnalysis = scanner.startPartitionAnalysis();

                    if (partAnalysis != 0) {
                        applicationWindow->showError("Partition Analysis FAILED!");
                        applicationWindow->setText(
                                PrintingResultService::getPartitionAnalysisResultForGui(partAnalysis));
                        onReset();
                    } else {
                        applicationWindow->setProgress(step);
                        int fileExtAnalysis;
                        fileExtAnalysis = scanner.startFileExtensionAnalysis();

                        if (fileExtAnalysis != 0) {
                            applicationWindow->showError("File Extension Analysis FAILED!");
                            applicationWindow->setText(
                                    PrintingResultService::getFileExtensionAnalysisResultForGui(fileExtAnalysis));
                            onReset();
                        } else {
                            applicationWindow->setProgress(step);
                            int malwareAnalysis;
                            malwareAnalysis = scanner.startMalwareAnalysis();

                            if (malwareAnalysis != 0) {
                                applicationWindow->showError("Virus Analysis FAILED!");
                                applicationWindow->setText(
                                        PrintingResultService::getVirusAnalysisResultForGui(malwareAnalysis));
                                onReset();
                            } else {
                                applicationWindow->setText(PrintingService::getProcessWentGoodTextForGui());

                                okButtonSignalConnection.disconnect();
                                okButtonSignalConnection = applicationWindow->okButton->signal_clicked().connect(
                                        sigc::mem_fun(*this,
                                                      &DuckyDetectorApplicationGui::onReset));
                            }
                        }
                    }
                }
            }
        }
    }


    applicationWindow->endLoading();

}
