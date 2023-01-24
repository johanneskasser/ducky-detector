#include "DuckyDetectorApplicationGui.hpp"

DuckyDetectorApplicationGui::DuckyDetectorApplicationGui() : Gtk::Application("fh.campuswien.duckyDetector",
    Gio::APPLICATION_FLAGS_NONE) {
}

Glib::RefPtr<DuckyDetectorApplicationGui> DuckyDetectorApplicationGui::create() {
    return Glib::RefPtr<DuckyDetectorApplicationGui>(new DuckyDetectorApplicationGui());
}

DuckyDetectorGui* DuckyDetectorApplicationGui::createApplicationWindow() {
    applicationWindow = new DuckyDetectorGui();
    add_window(*applicationWindow);
    applicationWindow->signal_hide().connect(sigc::bind<Gtk::Window*>(sigc::mem_fun(*this,
        &DuckyDetectorApplicationGui::onHideWindow), applicationWindow));

    runApplication();

    return applicationWindow;
}

void DuckyDetectorApplicationGui::on_activate() {
    applicationWindow = createApplicationWindow();
    applicationWindow->present();
    applicationWindow->fullscreen();
}

void DuckyDetectorApplicationGui::runApplication() {
    resetWindow();
    applicationWindow->showProgressBar();
    applicationWindow->setText(PrintingService::getLogoForGui() + PrintingService::getStartOfScanProcessTextForGui());

    okButtonSignalConnection.disconnect();
    okButtonSignalConnection = applicationWindow->okButton->signal_clicked().connect(sigc::mem_fun(*this,
        &DuckyDetectorApplicationGui::onStartScanProcessAndCheckSystemRequirements));

    detailsButtonSignalConnection.disconnect();

    cancelButtonSignalConnection.disconnect();
    cancelButtonSignalConnection = applicationWindow->cancelButton->signal_clicked().connect(sigc::mem_fun(*this,
        &DuckyDetectorApplicationGui::onCancelButtonClicked));
}

void DuckyDetectorApplicationGui::onStartScanProcessAndCheckSystemRequirements() {
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

void DuckyDetectorApplicationGui::onInitialPeripheryAnalysis() {
    int result;

    detailsButtonSignalConnection.disconnect();
    detailsButtonSignalConnection = applicationWindow->detailsButton->signal_clicked().connect(sigc::mem_fun(*this,
        &DuckyDetectorApplicationGui::onPeripheryDetails));

    applicationWindow->showInfoDialog();
    result = scanner.startPeripheryAnalysis(true);

    if (result != 0) {
        applicationWindow->showError("Periphery Analysis FAILED!");
        applicationWindow->setText(PrintingResultService::getPeripheryAnalysisResultForGui(result));
        onReset();
    } else {
        applicationWindow->setProgress(step);
        onSecondPeripheryAnalysis();
    }
}

void DuckyDetectorApplicationGui::onSecondPeripheryAnalysis() {
    int result;

    result = scanner.startPeripheryAnalysis(false);

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

void DuckyDetectorApplicationGui::onStartPartitionAnalysis() {
    int result;

    detailsButtonSignalConnection.disconnect();
    detailsButtonSignalConnection = applicationWindow->detailsButton->signal_clicked().connect(sigc::mem_fun(*this,
        &DuckyDetectorApplicationGui::onPartitionDetails));

    result = scanner.startPartitionAnalysis();

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

void DuckyDetectorApplicationGui::onStartFileExtensionAnalysis() {
    int result;

    detailsButtonSignalConnection.disconnect();
    detailsButtonSignalConnection = applicationWindow->detailsButton->signal_clicked().connect(sigc::mem_fun(*this,
        &DuckyDetectorApplicationGui::onFileExtensionDetails));

    result = scanner.startFileExtensionAnalysis();

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

void DuckyDetectorApplicationGui::onStartMalwareAnalysis() {
    int result;

    detailsButtonSignalConnection.disconnect();
    detailsButtonSignalConnection = applicationWindow->detailsButton->signal_clicked().connect(sigc::mem_fun(*this,
        &DuckyDetectorApplicationGui::onMalwareDetails));

    result = scanner.startMalwareAnalysis();

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
    applicationWindow->showDetailsDialog(PrintingDetailsService::getPreparationDetailsForGui(scanner.getPreparationHelperDetails()));
}

void DuckyDetectorApplicationGui::onUsbPreCheckDetails() {
    applicationWindow->showDetailsDialog(PrintingResultService::getUsbPreCheckResultForGui());
}

void DuckyDetectorApplicationGui::onPeripheryDetails() {
    applicationWindow->showDetailsDialog(PrintingDetailsService::getPeripheryAnalysisDetailsForGui(scanner.getUsbPeripheryAnalyserDetails(),
        scanner.getDevice()));
}

void DuckyDetectorApplicationGui::onPartitionDetails() {
    applicationWindow->showDetailsDialog(PrintingDetailsService::getPartitionAnalysisDetailsForGui(scanner.getPartitionAnalyserPartitionCounterDetails(),
        scanner.getFilesystems(), scanner.getPartitionAnalyserFlagsDetails()));
}

void DuckyDetectorApplicationGui::onFileExtensionDetails() {
    applicationWindow->showDetailsDialog(PrintingDetailsService::getFileExtensionAnalysisDetailsForGui(scanner.getFileExtensionAnalyserFileCounterDetails(),
        scanner.getFileExtensionAnalyserBlackListedFilesDetails()));
}

void DuckyDetectorApplicationGui::onMalwareDetails() {
    applicationWindow->showDetailsDialog(PrintingDetailsService::getVirusAnalysisDetailsForGui(scanner.getVirusAnalyserFileCounterDetails(),
        scanner.getVirusAnalyserNumberOfSignatureDetails(), scanner.getVirusAnalyserDataScannedInMbDetails(),
        scanner.getVirusAnalyserInfectedFilesCounterDetails(), scanner.getVirusAnalyserFoundVirusNamesDetails(),
        scanner.getVirusAnalyserInfectedFilesDetails(), scanner.getVirusAnalyserEngineStatusDetails()));
}

void DuckyDetectorApplicationGui::onCancelButtonClicked() {
    okButtonSignalConnection.disconnect();
    detailsButtonSignalConnection.disconnect();
    cancelButtonSignalConnection.disconnect();
    applicationWindow->setText(PrintingService::getUnblockingPortsTextForGui() + PrintingService::getByeTextForGui());
    scanner.freeMemory();
    applicationWindow->unset_application();
}

void DuckyDetectorApplicationGui::onHideWindow(Gtk::Window* window) {
    delete window;
}

DuckyDetectorApplicationGui::~DuckyDetectorApplicationGui() {
    delete applicationWindow;
}

void DuckyDetectorApplicationGui::resetWindow() {
    applicationWindow->hideInfoBar();
    applicationWindow->resetProgressBar();
}
