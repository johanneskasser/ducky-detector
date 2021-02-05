#ifndef FILEHANDLER_H // prevents multiple compiles
#define FILEHANDLER_H

#include <dirent.h>
#include <functional>
#include <fstream>
#include <iostream>

#include "../config/Settings.hpp"

class FileHandler {
    public:
        FileHandler();
        virtual ~FileHandler();
        void fileDiscovery(const std::string &path, bool fullPath, const std::function<void(const std::string&)>& callback);
        static bool readBitFile(char const* path) ;
        static bool writeBitFile(char const* path, bool state) ;
        static bool writeStringToFile(char const* path, char const* content) ;

    private:
        Settings settings;
};

#endif
