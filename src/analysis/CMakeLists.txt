add_library(Scanner Scanner.cpp)
add_library(PreparationHelper preparation/PreparationHelper.cpp)
add_library(UsbPeripheryAnalyser usbPeriphery/UsbPeripheryAnalyser.cpp)
add_library(PartitionAnalyser partition/PartitionAnalyser.cpp)
add_library(FileExtensionAnalyser fileExtension/FileExtensionAnalyser.cpp)
add_library(VirusAnalyser virus/MalwareAnalyserWithClamAv.cpp)