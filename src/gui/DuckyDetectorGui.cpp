#include "DuckyDetectorGui.hpp"
#include <string> 
using namespace std;

DuckyDetectorGui::DuckyDetectorGui() {
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("src/guiXML/duckyDetector.glade");


    // Initialize the GtkBuilder object
    // builder = gtk_builder_new_from_file("../guiXML/duckyDetector.glade");

    // builder->add_from_file("../guiXML/duckyDetector.glade");
    

    // infoBar = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    // builder->get_widget("buttonBox", buttonBox);

    // Get the widgets from the UI file
    builder->get_widget("rubberDuckyDetector", rubberDuckyDetector);
    builder->get_widget("box", box);
    builder->get_widget("infoBar", infoBar);
    builder->get_widget("infoBarMessage", infoBarMessage);
    builder->get_widget("textView", textView);
    builder->get_widget("okButton", okButton);
    builder->get_widget("detailsButton", detailsButton);
    builder->get_widget("cancelButton", cancelButton);
    builder->get_widget("buttonBox", buttonBox);
    builder->get_widget("scrolledWindow", scrolledWindow);
    builder->get_widget("progressBar", progressBar);
    //builder->get_object("textBuffer", textBuffer.operator->());

    //textBuffer = Glib::RefPtr<Gtk::TextBuffer>::cast_dynamic(builder->get_object("textBuffer"));

    textBuffer = Gtk::TextBuffer::create();

    (*textView).set_buffer(textBuffer);


    // Connect the signals
    // (*okButton).signal_clicked().connect(sigc::mem_fun(*this, &DuckyDetectorGui::onOkButtonClicked));
    // (*detailsButton).signal_clicked().connect(sigc::mem_fun(*this, &DuckyDetectorGui::onDetailsButtonClicked));
    //(*cancelButton).signal_clicked().connect(sigc::mem_fun(*this, &DuckyDetectorGui::onCancelButtonClicked));

    // Initialize the details dialog
    initDetailsDialog();

    (*rubberDuckyDetector).show_all_children();

    // Show the main window
    (*rubberDuckyDetector).show_all();
}

void DuckyDetectorGui::hideInfoBar() {
    (*infoBar).hide();
}

void DuckyDetectorGui::showDetailsDialog(const std::string& details) {
    detailsTextBuffer->set_text(details);
    detailsDialog->show_all_children();
    detailsDialog->fullscreen();
    detailsDialog->run();
}

void DuckyDetectorGui::showError(const std::string& text) {
    (*infoBarMessage).set_text(text);
    (*infoBar).set_message_type(Gtk::MESSAGE_ERROR);
    (*infoBar).show();
}

void DuckyDetectorGui::showSuccess(const std::string& text) {
    (*infoBarMessage).set_text(text);
    (*infoBar).set_message_type(Gtk::MESSAGE_INFO);
    (*infoBar).show();
}

void DuckyDetectorGui::showInfoDialog() {
    Gtk::MessageDialog dialog(*this, "Please insert usb drive and press OK.");
    dialog.set_title("Information");
    dialog.set_decorated(false);
    dialog.set_position(Gtk::WIN_POS_CENTER_ON_PARENT);
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
/*
void DuckyDetectorGui::initMainWindow() {
    set_default_size(320, 240);
    set_title(title);
    set_position(Gtk::WIN_POS_CENTER);
    set_decorated(false);
    set_border_width(5);

    (*textBuffer) = Gtk::TextBuffer::create();

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
}*/

void DuckyDetectorGui::initDetailsDialog() {
    // Initialize the GtkBuilder object
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();

    builder->add_from_file("/home/johannes/ducky-detector/src/guiXML/duckyDetector.glade");

    // Get the widgets from the details dialog UI file
    builder->get_widget("detailsDialog", detailsDialog);
    builder->get_widget("detailsTextView", detailsTextView);
    builder->get_widget("detailsBox", detailsBox);
    builder->get_widget("detailsOkButton", detailsOkButton);
    builder->get_widget("detailsButtonBox", detailsButtonBox);
    builder->get_widget("detailsScrolledWindow", detailsScrolledWindow);

    detailsTextBuffer = Glib::RefPtr<Gtk::TextBuffer>::cast_dynamic(builder->get_object("detailsTextBuffer"));

    detailsDialog->signal_hide().connect(sigc::mem_fun(*this, &DuckyDetectorGui::resetDetailsDialog));

    detailsOkButton->signal_clicked().connect(sigc::mem_fun(*detailsDialog, &Gtk::Window::hide));
}

DuckyDetectorGui::~DuckyDetectorGui() {
    delete detailsDialog;
}
