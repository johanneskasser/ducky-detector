#include "DuckyDetectorGui.hpp"

DuckyDetectorGui::DuckyDetectorGui() {
    // Initialize the GtkBuilder object
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();

    // Load the UI file
    try {
        builder->add_from_file("example.glade");
    }
    catch(const Glib::FileError& ex) {
        std::cerr << "FileError: " << ex.what() << std::endl;
        return;
    }
    catch(const Glib::MarkupError& ex) {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        return;
    }
    catch(const Gtk::BuilderError& ex) {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        return;
    }

    // Get the widgets from the UI file
    builder->get_widget("main_window", this);
    builder->get_widget("info_bar", &infoBar);
    builder->get_widget("info_bar_message", &infoBarMessage);
    builder->get_widget("text_view", &textView);
    builder->get_widget("ok_button", &okButton);
    builder->get_widget("details_button", &detailsButton);
    builder->get_widget("cancel_button", &cancelButton);
    builder->get_widget("button_box", &buttonBox);
    builder->get_widget("scrolled_window", &scrolledWindow);
    builder->get_widget("progress_bar", &progressBar);

    // Connect the signals
    okButton.signal_clicked().connect(sigc::mem_fun(*this, &DuckyDetectorGui::onOkButtonClicked));
    detailsButton.signal_clicked().connect(sigc::mem_fun(*this, &DuckyDetectorGui::onDetailsButtonClicked));
    cancelButton.signal_clicked().connect(sigc::mem_fun(*this, &DuckyDetectorGui::onCancelButtonClicked));

    // Initialize the details dialog
    initDetailsDialog();

    // Show the main window
    show_all();
}

void DuckyDetectorGui::hideInfoBar() {
    infoBar.hide();
}

void DuckyDetectorGui::showDetailsDialog(const std::string& details) {
    detailsTextBuffer->set_text(details);
    detailsDialog->show_all_children();
    detailsDialog->fullscreen();
    detailsDialog->run();
}

void DuckyDetectorGui::showError(const std::string& text) {
    infoBarMessage.set_text(text);
    infoBar.set_message_type(Gtk::MESSAGE_ERROR);
    infoBar.show();
}

void DuckyDetectorGui::showSuccess(const std::string& text) {
    infoBarMessage.set_text(text);
    infoBar.set_message_type(Gtk::MESSAGE_INFO);
    infoBar.show();
}

void DuckyDetectorGui::showInfoDialog() {
    Gtk::MessageDialog dialog(*this, "Please insert usb drive and press OK.");
    dialog.set_title("Information");
    dialog.set_decorated(false);
    dialog.set_position(Gtk::WIN_POS_CENTER_ON_PARENT);
    dialog.run();
}

void DuckyDetectorGui::showProgressBar() {
    progressBar.show();
}

void DuckyDetectorGui::setProgress(double progress) {
    double newProgress = progressBar.get_fraction() + progress;
    progressBar.set_fraction(newProgress);
}

void DuckyDetectorGui::resetDetailsDialog() {
    detailsTextBuffer->set_text("");
}

void DuckyDetectorGui::resetProgressBar() {
    progressBar.set_fraction(0.0);
}

void DuckyDetectorGui::setText(const std::string& text) {
    textBuffer->set_text(text);
}

void DuckyDetectorGui::setTextForReset(const std::string& text) {
    textBuffer->set_text(textBuffer->get_text() + text);
}

void DuckyDetectorGui::initMainWindow() {
    set_default_size(320, 240);
    set_title(title);
    set_position(Gtk::WIN_POS_CENTER);
    set_decorated(false);
    set_border_width(5);

    textBuffer = Gtk::TextBuffer::create();

    textView.set_editable(false);
    textView.set_cursor_visible(false);
    textView.set_buffer(textBuffer);

    scrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    scrolledWindow.add(textView);

    okButton.set_label("OK");
    detailsButton.set_label("Details");
    cancelButton.set_label("Cancel");

    buttonBox.pack_start(okButton);
    buttonBox.pack_start(detailsButton);
    buttonBox.pack_start(cancelButton);
    buttonBox.set_border_width(5);
    buttonBox.set_spacing(5);
    buttonBox.set_layout(Gtk::BUTTONBOX_CENTER);

    auto infoBarContainer = dynamic_cast<Gtk::Container*>(infoBar.get_content_area());
    if (infoBarContainer) {
        infoBarContainer->add(infoBarMessage);
    }

    progressBar.set_halign(Gtk::ALIGN_CENTER);
    progressBar.set_valign(Gtk::ALIGN_CENTER);
    progressBar.set_text("Analysing process");
    progressBar.set_show_text(true);

    box.set_orientation(Gtk::ORIENTATION_VERTICAL);
    box.pack_start(infoBar, Gtk::PACK_SHRINK);
    box.pack_start(scrolledWindow);
    box.pack_start(progressBar, Gtk::PACK_SHRINK);
    box.pack_start(buttonBox, Gtk::PACK_SHRINK);

    add(box);
    show_all_children();

    infoBar.hide();
    progressBar.hide();
}

void DuckyDetectorGui::initDetailsDialog() {
    // Initialize the GtkBuilder object
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();

    // Load the details dialog UI file
    try {
        builder->add_from_file("details_dialog.glade");
    }
    catch(const Glib::FileError& ex) {
        std::cerr << "FileError: " << ex.what() << std::endl;
        return;
    }
    catch(const Glib::MarkupError& ex) {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        return;
    }
    catch(const Gtk::BuilderError& ex) {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        return;
    }

    // Get the widgets from the details dialog UI file
    builder->get_widget("details_dialog", &detailsDialog);
    builder->get_widget("details_text_view", &detailsTextView);

    // Connect the signals
    detailsDialog->signal_response().connect(sigc::mem_fun(*this, &DuckyDetectorGui::onDetailsDialogResponse));
}

DuckyDetectorGui::~DuckyDetectorGui() {
    delete detailsDialog;
}
