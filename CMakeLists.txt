cmake_minimum_required(VERSION 3.16)
# set the projetct name
project(cmake-build-debug/DuckyDetector)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)

# add the executable
add_executable(duckyDetector src/Detector.cpp)

# add gtkmm
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

# find wiringPi library
find_library(WIRINGPI_LIBRARIES NAMES wiringPi)

# set the include directories for wiringPi
include_directories(/usr/local/include)

# add libraries
add_subdirectory(src/analysis)
add_subdirectory(src/application)
add_subdirectory(src/util)
add_subdirectory(src/gui)

# link the libraries
target_link_libraries(duckyDetector Scanner PreparationHelper UsbPeripheryAnalyser PartitionAnalyser
        FileExtensionAnalyser VirusAnalyser DuckyDetectorGui Device FileHandler Interface PrintingService Setup
        usb-1.0 clamav ${GTKMM_LIBRARIES} ${WIRINGPI_LIBRARIES} wiringPi)
