#include "DuckyDetectorGui.hpp"
#include <string>
#include <gtkmm/main.h>
#include <future>

using namespace std;

DuckyDetectorGui::DuckyDetectorGui() {
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("src/guiXML/duckyDetector.glade");

    // Get the widgets from the UI file
    builder->get_widget("rubberDuckyDetector", rubberDuckyDetector);
    builder->get_widget("box", box);
    builder->get_widget("textView", textView);
    builder->get_widget("okButton", okButton);
    builder->get_widget("detailsButton", detailsButton);
    builder->get_widget("cancelButton", cancelButton);
    builder->get_widget("buttonBox", buttonBox);
    builder->get_widget("scrolledWindow", scrolledWindow);
    builder->get_widget("progressBar", progressBar);
    builder->get_widget("currentModuleName", currentModuleName);
    builder->get_widget("moduleNameBox", moduleNameBox);
    builder->get_widget("loading-indicator", loadingIndicator);
    builder->get_widget("fast_run", fastRun);
    builder->get_widget("fastRunBottonBox", buttonBoxFastRun);

    textBuffer = Gtk::TextBuffer::create();

    (*textView).set_buffer(textBuffer);
    initDetailsDialog();

    (*rubberDuckyDetector).show_all_children();

    // Show the main window
    (*rubberDuckyDetector).show_all();

    (*currentModuleName).set_text("Rubber Ducky Detector");

    (*rubberDuckyDetector).fullscreen();
}
/*
void DuckyDetectorGui::hideInfoBar() {
    (*infoBar).hide();
}*/

void DuckyDetectorGui::showDetailsDialog(const std::string& details) {
    bean1buffer->set_text("");
    bean2buffer->set_text("");
    bean3buffer->set_text("");
    bean4buffer->set_text("");


    std::vector<std::string> substrings;
    std::size_t index = 0;
    std::string newS = details;
    if(newS.find('\\') == std::string::npos) {
        bean1buffer->set_text("/");
        bean2buffer->set_text("/");
        bean3buffer->set_text("/");
        bean4buffer->set_text("/");

    } else {
        while((index = newS.find('\\')) != std::string::npos) {
            substrings.push_back(newS.substr(0, index));
            //unsigned long ul = static_cast<unsigned long>(index); 
            newS.erase(0, index + 2);
        }

        if(substrings.size() >= 4) {
            bean1buffer->set_text(substrings[0]);
            bean2buffer->set_text(substrings[1]);
            bean3buffer->set_text(substrings[2]);
            bean4buffer->set_text(substrings[3]);
        } else if (substrings.size() == 3) {
            bean1buffer->set_text(substrings[0]);
            bean2buffer->set_text(substrings[1]);
            bean3buffer->set_text(substrings[2]);
            bean4buffer->set_text("/");
        } else if (substrings.size() == 2) {
            bean1buffer->set_text(substrings[0]);
            bean2buffer->set_text(substrings[1]);
            bean3buffer->set_text("/");
            bean4buffer->set_text("/");
        } else if (substrings.size() == 1) {
            bean1buffer->set_text(substrings[0]);
            bean2buffer->set_text("/");
            bean3buffer->set_text("/");
            bean4buffer->set_text("/");
        }
        

    }


    detailsTextBuffer->set_text(newS);
    detailsDialog->show_all_children();
    detailsDialog->fullscreen();
    detailsDialog->run();
}

void DuckyDetectorGui::showError(const std::string& text) {
    Gdk::RGBA color;
    color.set_rgba(1.0, 0.0, 0.0, 1.0);
    (*moduleNameBox).override_background_color(color);
}

void DuckyDetectorGui::showSuccess(const std::string& text) {
    Gdk::RGBA color;
    color.set_rgba(0.0, 1.0, 0.0, 1.0);
    (*moduleNameBox).override_background_color(color);
}

void DuckyDetectorGui::setModuleName(const std::string& moduleName) {
    (*currentModuleName).set_text(moduleName);
}

void DuckyDetectorGui::showInfoDialog() {
    Gtk::MessageDialog dialog(*this, "Please insert usb drive and press OK.");
    dialog.set_title("Information");
    dialog.set_decorated(false);
    dialog.set_position(Gtk::WIN_POS_CENTER);
    dialog.run();
}

void DuckyDetectorGui::showProgressBar() {
    (*progressBar).show();
}

void DuckyDetectorGui::setProgress(double progress) {
    double newProgress = (*progressBar).get_fraction() + progress;
    (*progressBar).set_fraction(newProgress);
}

void DuckyDetectorGui::resetDetailsDialog() {
    detailsTextBuffer->set_text("");
    bean1buffer->set_text("");
    bean2buffer->set_text("");
    bean3buffer->set_text("");
    bean4buffer->set_text("");
}

void DuckyDetectorGui::resetProgressBar() {
    (*progressBar).set_fraction(0.0);
}

void DuckyDetectorGui::setText(const std::string& text) {
    textBuffer->set_text(text);
    cout << textBuffer->get_text();
    textView->queue_draw();
}

void DuckyDetectorGui::setTextForReset(const std::string& text) {
    textBuffer->set_text(textBuffer->get_text() + text);
}

void DuckyDetectorGui::initDetailsDialog() {
    // Initialize the GtkBuilder object
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();

    builder->add_from_file("src/guiXML/duckyDetector.glade");

    // Get the widgets from the details dialog UI file
    builder->get_widget("detailsDialog", detailsDialog);
    builder->get_widget("detailsTextView", detailsTextView);
    builder->get_widget("detailsBox", detailsBox);
    builder->get_widget("detailsOkButton", detailsOkButton);
    builder->get_widget("detailsButtonBox", detailsButtonBox);
    builder->get_widget("detailsScrolledWindow", detailsScrolledWindow);
    builder->get_widget("bean1", bean1);
    builder->get_widget("bean2", bean2);
    builder->get_widget("bean3", bean3);
    builder->get_widget("bean4", bean4);

    detailsTextBuffer = Glib::RefPtr<Gtk::TextBuffer>::cast_dynamic(builder->get_object("detailsTextBuffer"));
    bean1buffer = Glib::RefPtr<Gtk::TextBuffer>::cast_dynamic(builder->get_object("bean1buffer"));
    bean2buffer = Glib::RefPtr<Gtk::TextBuffer>::cast_dynamic(builder->get_object("bean2buffer"));
    bean3buffer = Glib::RefPtr<Gtk::TextBuffer>::cast_dynamic(builder->get_object("bean3buffer"));
    bean4buffer = Glib::RefPtr<Gtk::TextBuffer>::cast_dynamic(builder->get_object("bean4buffer"));


    detailsDialog->signal_hide().connect(sigc::mem_fun(*this, &DuckyDetectorGui::resetDetailsDialog));

    detailsOkButton->signal_clicked().connect(sigc::mem_fun(*detailsDialog, &Gtk::Window::hide));
}

void DuckyDetectorGui::startLoading() {
    (*loadingIndicator).show();
    (*loadingIndicator).start();
}

void DuckyDetectorGui::endLoading() {
    (*loadingIndicator).stop();
    (*loadingIndicator).hide();
}

template<typename Func>
int DuckyDetectorGui::executeBackgroundTask(Func func) {
    startLoading();

    std::future<int> future = std::async(std::launch::async, func);

    while(future.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
        while (Gtk::Main::events_pending()) {
            Gtk::Main::iteration();
        }
    }
    endLoading();
    return future.get();
}

void DuckyDetectorGui::resetModuleNameBackground() {
    (*moduleNameBox).unset_background_color();
}

bool DuckyDetectorGui::returnFastRunStatus() {
    if((*fastRun).get_active() == true) {
        ::printf("\nFast Run: true");
    } else
    {
        ::printf("\nFast Run: false");
    }
    return (*fastRun).get_active();
}

DuckyDetectorGui::~DuckyDetectorGui() {
    delete detailsDialog;
}
