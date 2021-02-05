#ifndef DUCKYDETECTOR_FILEEXTENSIONANALYSER_HPP
#define DUCKYDETECTOR_FILEEXTENSIONANALYSER_HPP

#include <set>
#include <string>
#include "../../util/Device.hpp"
#include "../../util/FileHandler.hpp"

class FileExtensionAnalyser {
    public:
        explicit FileExtensionAnalyser(Device* device);
        virtual ~FileExtensionAnalyser();

        /**
         * Optional analysis of file/s on USB device
         *
         * @param partitions
         * @param fileSystems
         * @return
         * 0 - OK, if no critical file/s were found
         * 1 - Critical file/s found!
         * 2 - Unmounting went wrong!
         * 3 - Mounting went wrong!
         */
        int analyseFileExtensions(std::string* partitions, std::string* filesystems);
        std::set<std::string> getBlackListedFiles();
        int getFileCounter();

    private:
        int fileCounter = 0;
        std::set<std::string> blackListedFiles;
        Device* device;
        FileHandler fileHandler;
        Settings settings;

        /**
         * Checks if the given file is blacklisted
         *
         * @param filename
         */
        void fileExtensionBlacklistChecker(const std::string& filename);
        /**
         * Retrieves the file extension of given file
         *
         * @param filename
         * @return
         */
        static std::string getExtensionOfFile(const std::string& filename);
};

#endif //DUCKYDETECTOR_FILEEXTENSIONANALYSER_HPP
