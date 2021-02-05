#include "UsbPeripheryAnalyser.hpp"

UsbPeripheryAnalyser::UsbPeripheryAnalyser(Device* pDevice) {
    device = pDevice;
}

int UsbPeripheryAnalyser::peripheryAnalysis(bool initial, bool runWithGui) {
    if (initial) {
        device_found = false;
        device_parameters_changed = false;
        device_interfaces_changed = false;
        device_vanished = false;
        multiple_devices_found = false;
        multiple_interfaces_found = false;
        results_error = false;
        device->setInitializationStatus(false);
    }

    monitorUSBPeriphery(initial ? stage0Timeout : stage1Timeout, initial, runWithGui);

    return usbPeripheryAnalysis();
}

std::map<std::string, bool> UsbPeripheryAnalyser::getUsbPeripheryAnalyserDetails() {
    usbPeripheryAnalyserDetails.insert(std::make_pair("deviceFound", device_found));
    usbPeripheryAnalyserDetails.insert(std::make_pair("multipleDevicesFound", multiple_devices_found));
    usbPeripheryAnalyserDetails.insert(std::make_pair("deviceParameterChanged", device_parameters_changed));
    usbPeripheryAnalyserDetails.insert(std::make_pair("deviceVanished", device_vanished));
    usbPeripheryAnalyserDetails.insert(std::make_pair("multipleInterfacesFound", multiple_devices_found));
    usbPeripheryAnalyserDetails.insert(std::make_pair("passedUsbPeripheryAnalysis", passedUsbPeripheryAnalysis));
    usbPeripheryAnalyserDetails.insert(std::make_pair("peripheryErrors", results_error));

    return usbPeripheryAnalyserDetails;
}

bool UsbPeripheryAnalyser::getDevice(libusb_device *dev) {
    libusb_device_descriptor desc;
    int result = libusb_get_device_descriptor(dev, &desc);

    if (result < 0) {
        // failed to get device descriptor
        return false;
    }

    // is the device connected to one of the three USB ports? we do not get any other devices
    // port path of a given device
    uint8_t portList[7];
    // length of the port-path
    uint8_t portListLen = libusb_get_port_numbers(dev, portList, 7);

    // 1. DEVICE CONNECTED TO USB PORTS? - check if the device is connected to one of the 4 ports of the raspberry pi
    if (device->detectDevice(dev)) {
        libusb_config_descriptor *config;
        libusb_get_config_descriptor(dev, 0, &config);

        // 2. FIRST DEVICE? - check if this is the first detection
        if (device->getInitializationStatus()) { // NO

            // we already found a device, check if device parameters changed
            if (device->compareDevice((int)desc.idVendor, (int)desc.idProduct, (int)desc.bDeviceClass,
                    (int)desc.bDeviceSubClass, (int)desc.bDeviceProtocol, (int)portListLen, (int)desc.bNumConfigurations,
                    (int)config->bNumInterfaces)) {
                device_found = true;

                // device seems the same - now check if interface parameter changed
                const libusb_interface *inter;
                const libusb_interface_descriptor *interdesc;
                const libusb_endpoint_descriptor *epdesc;

                for (int i = 0; i < (int)config->bNumInterfaces; i++) {
                    inter = &config->interface[i];

                    for (int j = 0; j < inter->num_altsetting; j++) {
                        interdesc = &inter->altsetting[j];

                        if (!device->compareInterfaces((int)interdesc->bInterfaceClass, (int)interdesc->bNumEndpoints)) {
                            device_interfaces_changed = true;
                        }

                        for (int k = 0; k < (int)interdesc->bNumEndpoints; k++) {
                            // we ignore endpoints atm
                            epdesc = &interdesc->endpoint[k];
                        }
                    }
                }

                // everything looks good, update device (specific. portListLen, portList)
                device->updateDescriptor((int)desc.idVendor, (int)desc.idProduct, (int)desc.bDeviceClass, (int)desc.bDeviceSubClass,
                    (int)desc.bDeviceProtocol, (int)portListLen, (int)desc.bNumConfigurations, (int)config->bNumInterfaces, portList);

            } else { // device parameters changed
                device_parameters_changed = true;
            }
        } else { // YES
            // this is our first discovery. save this device
            device->setDescriptor((int)desc.idVendor, (int)desc.idProduct, (int)desc.bDeviceClass, (int)desc.bDeviceSubClass, (int)desc.bDeviceProtocol, (int)portListLen, (int)desc.bNumConfigurations, (int)config->bNumInterfaces, portList);
            device_found = true;

            // save interfaces
            const libusb_interface *inter;
            const libusb_interface_descriptor *interdesc;
            const libusb_endpoint_descriptor *epdesc;

            for (int i = 0; i < (int)config->bNumInterfaces; i++) {
                inter = &config->interface[i];

                for (int j = 0; j < inter->num_altsetting; j++) {
                    interdesc = &inter->altsetting[j];
                    device->addInterface((int)interdesc->bInterfaceClass, (int)interdesc->bNumEndpoints); // add interface

                    for (int k = 0; k < (int)interdesc->bNumEndpoints; k++) {
                        // we ignore endpoints atm
                        epdesc = &interdesc->endpoint[k];
                    }
                }
            }
        }

        libusb_free_config_descriptor(config);
        // found a device connected to one of the 4 ports
        return true;
    } else {
        // no device could be detected. did we already have a detection?
        return false;
    }
}

void UsbPeripheryAnalyser::monitorUSBPeriphery(int timeout, bool initial, bool runWithGui) {
	std::clock_t start;
	// timestamp of last GUI update
	std::clock_t frame;
    double duration = 0;
	// "FPS" - how often does the GUI update during scan
	const double updateRate = 0.05;

    start = std::clock();
	frame = start;
	do {
	    if (!runWithGui) {
            if (((std::clock() - frame) / (double) CLOCKS_PER_SEC) >= updateRate) {
                // clear terminal with ANSI escape codes
                std::cout << "\033[2J\033[1;1H";
                std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
                if (initial && !device_found) {
                    std::cout << "                 ┌--------------------------┐" << std::endl;
                    std::cout << "                 |  Connect USB device NOW  |" << std::endl;
                    std::cout << "                 └--------------------------┘" << std::endl;
                } else {
                    std::cout << std::endl << std::endl << std::endl;
                }
                std::cout << "                  Detection in Progress: " << (int) (duration / timeout * 100) << "%"
                          << std::endl; // (" << duration << "sek)" << endl;
                frame = std::clock();
            }
        }

		libusb_device **devices;
		libusb_context *context = nullptr;
		// holds the number of usb devices connected on one of the 4 ports
		int usb_device_counter = 0;

		if (libusb_init(&context) < 0) {
			results_error = true;
			break;
		}

		// set verbosity level to 3, as suggested in the documentation
		libusb_set_option(context, libusb_option::LIBUSB_OPTION_LOG_LEVEL);
		// holds the number of all found devices (also internal ones)
        ssize_t device_counter = libusb_get_device_list(context, &devices);
		
        if (device_counter < 0) {
			results_error = true;
		}

		//for iterating through the list
		for (ssize_t i = 0; i < device_counter; i++) {
			// returns true if a device is connected
			if (getDevice(devices[i])) {
				usb_device_counter++;
			}
		}

		if (usb_device_counter > 1) {
            multiple_devices_found = true;
		}

		if (usb_device_counter == 0 && device_found) {
            device_vanished = true;
		}

		// free the list, unref the devices in it
		libusb_free_device_list(devices, 1);
		// close the session
		libusb_exit(context);

		duration = ((std::clock() - start) / (double) CLOCKS_PER_SEC);
		
	} while (duration < timeout);
}

int UsbPeripheryAnalyser::usbPeripheryAnalysis() {
    if (!device_found) {
		return 1;
	}

	// stage 0 / stage 1
	if (device_found && !multiple_devices_found && !device_parameters_changed && !device_interfaces_changed && !device_vanished && !results_error) {
        // now check all interfaces
        int interfaces[device->getbNumInterfaces()];
        int *ifaces = device->getbNumInterfaceClasses(interfaces);
        bool redInterfaceFlag = true;

        if (device->getbNumInterfaces() != 1) {
            redInterfaceFlag = false;
            multiple_interfaces_found = true;
        }

        for (int i = 0; i < device->getbNumInterfaces(); i++) {
            if (ifaces[i] != 8) {
                passedUsbPeripheryAnalysis = false;
                redInterfaceFlag = false;
            }
        }

        if (!redInterfaceFlag) {
            passedUsbPeripheryAnalysis = false;
            return 2;
        } else {
            passedUsbPeripheryAnalysis = true;
            // returns true if everything looks good at a given moment
            return 0;
        }
	} else {
        passedUsbPeripheryAnalysis = false;
        return 3;
	}
}

UsbPeripheryAnalyser::~UsbPeripheryAnalyser() = default;
