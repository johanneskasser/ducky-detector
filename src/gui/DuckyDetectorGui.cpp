#include "DuckyDetectorGui.hpp"

DuckyDetectorGui::DuckyDetectorGui() {
    initDetailsDialog();
    initMainWindow();
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
    detailsDialog = new Gtk::MessageDialog(*this, "Details", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_NONE);
    detailsDialog->set_default_size(320, 240);
    detailsDialog->set_title("Details");
    detailsDialog->set_position(Gtk::WIN_POS_CENTER_ON_PARENT);
    detailsDialog->set_decorated(false);
    detailsDialog->set_border_width(5);
    detailsDialog->signal_hide().connect(sigc::mem_fun(*this, &DuckyDetectorGui::resetDetailsDialog));

    detailsTextBuffer = Gtk::TextBuffer::create();

    detailsTextView.set_editable(false);
    detailsTextView.set_cursor_visible(false);
    detailsTextView.set_buffer(detailsTextBuffer);

    detailsScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    detailsScrolledWindow.add(detailsTextView);

    detailsOkButton.set_label("OK");
    detailsOkButton.signal_clicked().connect(sigc::mem_fun(detailsDialog, &Gtk::Window::hide));

    detailsButtonBox.pack_start(detailsOkButton);
    detailsButtonBox.set_border_width(5);
    detailsButtonBox.set_spacing(5);
    detailsButtonBox.set_layout(Gtk::BUTTONBOX_CENTER);

    detailsBox.set_orientation(Gtk::ORIENTATION_VERTICAL);
    detailsBox.pack_start(detailsScrolledWindow);
    detailsBox.pack_start(detailsButtonBox, Gtk::PACK_SHRINK);

    detailsDialog->get_content_area()->pack_start(detailsBox);
}

DuckyDetectorGui::~DuckyDetectorGui() {
    delete detailsDialog;
}
