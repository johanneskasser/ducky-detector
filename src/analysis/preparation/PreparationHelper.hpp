#ifndef DUCKYDETECTOR_PREPARATION_HPP
#define DUCKYDETECTOR_PREPARATION_HPP

#include "libusb-1.0/libusb.h"
#include <map>
#include "../../util/Device.hpp"
#include "../../config/Settings.hpp"
#include "../../util/Setup.hpp"

class PreparationHelper {
    public:
        PreparationHelper(Device* pDevice);
        virtual ~PreparationHelper();

        bool checkSystemRequirements();
        void restoreSystemState();
        /**
          * Checks if the usb ports are in use
          *
          * @return
          * true - All four ports are free
          * false - One or more ports are in use
          */
        bool usbPreCheck();
        std::map<std::string, bool> getPreparationHelperDetails();
    
    private:
        std::map<std::string, bool> preparationHelperDetails;
        Device* device;
        Setup setup;
        Settings settings;

        /**
         * Cleanup the open connections via the libusb library
         *
         * @param context
         * @param devices
         * @param deviceCounter
         */
        static void cleanupPreparationHelper(libusb_context *context, libusb_device **devices, ssize_t deviceCounter);
};

#endif //DUCKYDETECTOR_PREPARATION_HPP