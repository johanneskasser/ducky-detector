#ifndef DUCKYDETECTOR_DUCKYDETECTORAPPLICATION_HPP
#define DUCKYDETECTOR_DUCKYDETECTORAPPLICATION_HPP


#include <gtkmm/application.h>
#include "../gui/DuckyDetectorGui.hpp"
#include "../analysis/Scanner.hpp"
#include "../util/printing/PrintingService.hpp"
#include "../util/printing/PrintingResultService.hpp"
#include "../util/printing/PrintingDetailsService.hpp"

class DuckyDetectorApplicationGui : public Gtk::Application {
    public:
        DuckyDetectorApplicationGui();
        virtual ~DuckyDetectorApplicationGui();

        static Glib::RefPtr<DuckyDetectorApplicationGui> create();
        void on_activate() override;

    private:
        double step = 1.0/7.0;

        Scanner scanner;
        DuckyDetectorGui* applicationWindow;
        sigc::connection okButtonSignalConnection;
        sigc::connection detailsButtonSignalConnection;
        sigc::connection cancelButtonSignalConnection;

        DuckyDetectorGui* createApplicationWindow();
        void runApplication();
        void runApplicationInterceptor();
        void onStartScanProcessAndCheckSystemRequirements();
        void onUsbPreCheck();
        void onInitialPeripheryAnalysis();
        void onSecondPeripheryAnalysis();
        void onStartPartitionAnalysis();
        void onStartFileExtensionAnalysis();
        void onStartMalwareAnalysis();
        void onReset();
        void onPreparationDetails();
        void onUsbPreCheckDetails();
        void onPeripheryDetails();
        void onPartitionDetails();
        void onFileExtensionDetails();
        void onMalwareDetails();
        void onHideWindow(Gtk::Window* window);
        void onCancelButtonClicked();
        void resetWindow();
        void onFastAnalysis();
};

#endif //DUCKYDETECTOR_DUCKYDETECTORAPPLICATION_HPP
