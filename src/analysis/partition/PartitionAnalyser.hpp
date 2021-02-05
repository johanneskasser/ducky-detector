#ifndef DUCKYDETECTOR_PARTITIONANAYLSER_HPP // prevents multiple compiles
#define DUCKYDETECTOR_PARTITIONANAYLSER_HPP

#include "../../util/Device.hpp"

class PartitionAnalyser {
    public:
        explicit PartitionAnalyser(Device* pDevice);
        virtual ~PartitionAnalyser();

        /**
         * Optional analyis of partitions on USB device
         * Returns the name of the partition as parameter pointer
         *
         * @param partitions
         * @param fileSystems
         * @return
         * 0 - OK, if exactly one partition was found.
         * 1 - Authorizing failed!
         * 2 - Binding failed!
         * 3 - Could not read partition!
         * 4 - No or multiple partitions found!
         * 5 - Critical partition found!
         */
        int analysePartition(std::string* partitions, std::string* fileSystems);
        std::string* getFlags();
        int getPartitionCounter();
    
    private:
        int partitionCounter = 0;
        std::string flags[20];
        Device* device;
        Settings settings;

        /**
         * Returns number of partitions and name of partitions as out parameter
         *
         * @param partitions
         * @param filesystems
         * @param flags
         * @return the number of found partition/s
         */
        int getPartitions(std::string* partitions, std::string* filesystems);
        /**
         * Analysed the partition according to filesystems and flags
         *
         * @param fileSystems
         * @return
         * 0 - OK, if exactly one partition was found.
         * 4 - No or multiple partitions found!
         * 5 - Critical partition found!
         */
        int partitionAnalysis(std::string* fileSystems);
};

#endif //DUCKYDETECTOR_PARTITIONANAYLSER_HPP
