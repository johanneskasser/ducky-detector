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
#include <iostream>

class DuckyDetectorGui: public Gtk::ApplicationWindow {
    public:
        Gtk::Button *okButton, *cancelButton, *detailsButton;
        // Gtk::Builder builder = Gtk::Builder::Builder();
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
        //Gtk::ApplicationWindow* getApplicationWindow();

    private:
        std::string title = "Rubber Ducky Detector";
        Gtk::Box* box;
        Gtk::ButtonBox* buttonBox;
        Gtk::InfoBar* infoBar;
        Gtk::Label* infoBarMessage;
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

        void initMainWindow();
        void initDetailsDialog();
};

#endif //DUCKYDETECTOR_DUCKYDETECTORGUI_HPP
