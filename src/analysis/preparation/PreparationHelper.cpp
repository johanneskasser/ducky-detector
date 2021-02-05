#include "PreparationHelper.hpp"

PreparationHelper::PreparationHelper(Device* pDevice) {
    device = pDevice;
}

bool PreparationHelper::checkSystemRequirements() {
    if (!setup.getSystemState()) {
        setup.setDefaultInterfaceAuth(false);
        setup.setDefaultAuth(false);
        setup.setModProbe(false);
    }

    return setup.getSystemState();
}

void PreparationHelper::restoreSystemState() {
    setup.setDefaultInterfaceAuth(true);
    setup.setDefaultAuth(true);
    setup.setModProbe(true);
}

bool PreparationHelper::usbPreCheck() {
    // holds the list of devices
    libusb_device **devices = nullptr;
    libusb_context *context = nullptr;

    if (libusb_init(&context) < 0) {
        cleanupPreparationHelper(context, devices, 0);
        return false;
    }

    // set verbosity level to 3, as suggested in the documentation
    libusb_set_option(context, libusb_option::LIBUSB_OPTION_LOG_LEVEL);
	// holds the number of all found devices (also internal ones)
	ssize_t deviceCounter = libusb_get_device_list(context, &devices);

	if (deviceCounter < 0) {
        cleanupPreparationHelper(context, devices, deviceCounter);
		return false;
	}

	// iterating through the list
    for (int i = 0; i < deviceCounter; i++) {
        // check if a port is in use
	    if (device->detectDevice(devices[i])) {
            cleanupPreparationHelper(context, devices, deviceCounter);
	        return false;
	    }
	}

	// return true if all 4 ports are free
    cleanupPreparationHelper(context, devices, deviceCounter);
	return true;
}

std::map<std::string, bool> PreparationHelper::getPreparationHelperDetails() {
    preparationHelperDetails.insert(std::make_pair("modProbeDisabled", setup.isModProbeDisabled()));
    preparationHelperDetails.insert(std::make_pair("defaultAuthDisabled", setup.isDefaultAuthDisabled()));
    preparationHelperDetails.insert(std::make_pair("defaultInterfaceAuthDisabled", setup.isDefaultInterfaceAuthDisabled()));

    return preparationHelperDetails;
}

void PreparationHelper::cleanupPreparationHelper(libusb_context* context, libusb_device** devices, ssize_t deviceCounter) {
    // free the list, unref the devices in it
    if (deviceCounter > 0) {
        libusb_free_device_list(devices, deviceCounter);
    }

    // close the session
    libusb_exit(context);
}

PreparationHelper::~PreparationHelper() = default;
