#include "Setup.hpp"

Setup::Setup() = default;

bool Setup::isDefaultAuthDisabled() {
    return FileHandler::readBitFile(settings.defaultAuthFilePathUsbBus1) && FileHandler::readBitFile(settings.defaultAuthFilePathUsbBus2);
}

void Setup::setDefaultAuth(bool state) {
    FileHandler::writeBitFile(settings.defaultAuthFilePathUsbBus1, state);
    FileHandler::writeBitFile(settings.defaultAuthFilePathUsbBus2, state);
}

bool Setup::isDefaultInterfaceAuthDisabled() {
    return FileHandler::readBitFile(settings.defaultAuthFilePathUsbBus1) && FileHandler::readBitFile(settings.defaultAuthFilePathUsbBus2);
}

void Setup::setDefaultInterfaceAuth(bool state) {
    FileHandler::writeBitFile(settings.defaultInterfaceAuthFilePathUsbBus1, state);
    FileHandler::writeBitFile(settings.defaultInterfaceAuthFilePathUsbBus2, state);
}

bool Setup::isModProbeDisabled() {
	return FileHandler::readBitFile(settings.modProbeFilePath);
}

void Setup::setModProbe(bool state) {
    FileHandler::writeBitFile(settings.modProbeFilePath, state);
}

bool Setup::getSystemState() {
    // What switches are needed to start scanning securely?
    return !isDefaultInterfaceAuthDisabled() && !isDefaultAuthDisabled() && !isModProbeDisabled();
}

Setup::~Setup() = default;
