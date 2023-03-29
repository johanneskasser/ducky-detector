#ifndef DUCKYDETECTOR_DUCKYDETECTORGUI_HPP
#define DUCKYDETECTOR_DUCKYDETECTORGUI_HPP

#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/infobar.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/textview.h>
#include <gtkmm/builder.h>
#include <gtkmm/spinner.h>
#include <iostream>

class DuckyDetectorGui: public Gtk::ApplicationWindow {
    public:
        Gtk::Button *okButton, *cancelButton, *detailsButton;
        Gtk::ApplicationWindow* rubberDuckyDetector;
    

        DuckyDetectorGui();
        virtual ~DuckyDetectorGui();

        void hideInfoBar();
        void showDetailsDialog(const std::string& details);
        void showError(const std::string& text);
        void showSuccess(const std::string& text);
        void showInfoDialog();
        void showProgressBar();
        void setProgress(double progress);
        void resetProgressBar();
        void resetDetailsDialog();
        void setText(const std::string& text);
        void setTextForReset(const std::string& text);
        void setModuleName(const std::string& moduleName);
        void resetModuleNameBackground();
        void startLoading();
        void endLoading();
        //Gtk::ApplicationWindow* getApplicationWindow();

    private:
        std::string title = "Rubber Ducky Detector";
        Gtk::Box* box;
        Gtk::ButtonBox* buttonBox;
        Gtk::ProgressBar* progressBar;
        Gtk::ScrolledWindow* scrolledWindow;
        Glib::RefPtr<Gtk::TextBuffer> textBuffer;
        Gtk::TextView* textView;
        Gtk::MessageDialog* detailsDialog;
        Gtk::Box* detailsBox;
        Gtk::Button* detailsOkButton;
        Gtk::ButtonBox* detailsButtonBox;
        Gtk::ScrolledWindow* detailsScrolledWindow;
        Glib::RefPtr<Gtk::TextBuffer> detailsTextBuffer;
        Gtk::TextView* detailsTextView;
        Gtk::Label* currentModuleName;
        Gtk::Box* moduleNameBox;
        Gtk::TextView* bean1;
        Gtk::TextView* bean2;
        Gtk::TextView* bean3;
        Gtk::TextView* bean4;
        Glib::RefPtr<Gtk::TextBuffer> bean1buffer;
        Glib::RefPtr<Gtk::TextBuffer> bean2buffer;
        Glib::RefPtr<Gtk::TextBuffer> bean3buffer;
        Glib::RefPtr<Gtk::TextBuffer> bean4buffer;
        Gtk::Spinner* loadingIndicator;

        void initDetailsDialog();
};

#endif //DUCKYDETECTOR_DUCKYDETECTORGUI_HPP
