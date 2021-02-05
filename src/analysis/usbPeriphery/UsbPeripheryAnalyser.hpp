#ifndef DUCKYDETECTOR_USBPERIPHERYANALYSER_HPP // prevents multiple compiles
#define DUCKYDETECTOR_USBPERIPHERYANALYSER_HPP

#include <ctime>
#include <libusb-1.0/libusb.h>
#include <map>
#include "../../util/Device.hpp"

class UsbPeripheryAnalyser {
    public:
        explicit UsbPeripheryAnalyser(Device* pDevice);
        virtual ~UsbPeripheryAnalyser();
        /**
         * Analysis of the USB periphery
         *
         * @param initial
         * true - waits for device to plugin
         * false - waits for a given time, no changes allowed
         * @return
         * 0 - Everything went good
         * 1 - No devices were found!
         * 2 - Critical interface found!
         * 3 - Multiple problems found!
         */
        int peripheryAnalysis(bool initial, bool runWithGui);
        std::map<std::string, bool> getUsbPeripheryAnalyserDetails();

    private:
        Device* device;
        Settings settings;
        std::map<std::string, bool> usbPeripheryAnalyserDetails;
        // Stage 0 / Stage 1 - device descriptor checks
        bool device_found = false;
        bool device_parameters_changed = false;
        bool device_interfaces_changed = false;
        bool device_vanished = false;
        bool results_error = false;
        bool multiple_devices_found = false;
        bool multiple_interfaces_found = false;
        // is set to TRUE after stage 1 passed (no changes during stage 0 and stage 1 allowed)
        bool passedUsbPeripheryAnalysis = false;
        const int stage0Timeout = settings.stage0Timeout;
        const int stage1Timeout = settings.stage1Timeout;

        /**
         * Gets the plugged in device
         *
         * @param dev
         * @return
         * true - Device found
         * false - No device found
         */
        bool getDevice(libusb_device *dev);
        /**
         * Monitors the USB periphery
         *
         * @param timeout
         * @param initial
         */
        void monitorUSBPeriphery(int timeout, bool initial, bool runWithGui);
        /**
         * Analysis of the USB periphery
         *
         * @return
         * 0 - Everything went good
         * 1 - No devices were found!
         * 2 - Critical interface found!
         * 3 - Multiple problems found!
         */
        int usbPeripheryAnalysis();
};

#endif //DUCKYDETECTOR_USBPERIPHERYANALYSER_HPP