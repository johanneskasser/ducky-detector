#ifndef SETUP_H // prevents multiple compiles
#define SETUP_H

#include <iostream>
#include "FileHandler.hpp"
#include "../config/Settings.hpp"

class Setup {

    public:
        Setup();
        virtual ~Setup();

        bool isDefaultAuthDisabled();
        void setDefaultAuth(bool state);
        bool isDefaultInterfaceAuthDisabled();
        void setDefaultInterfaceAuth(bool state);
        bool isModProbeDisabled();
        void setModProbe(bool state);
        bool getSystemState();

    private:
        Settings settings;
};

#endif