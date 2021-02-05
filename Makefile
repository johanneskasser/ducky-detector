baseDir = $(shell pwd)
srcDir = $(baseDir)/src
buildDir = $(baseDir)/build
analysisDir = $(srcDir)/analysis
appDir = $(srcDir)/application
guiDir = $(srcDir)/gui
utilDir = $(srcDir)/util
printDir = $(utilDir)/printing
configDir = $(srcDir)/config

CXX := g++
CXX_FLAGS := -std=c++11
OBJECTS := duckyDetectorApplicationGui.o duckyDetectorApplicationTerminal.o \
scanner.o preparationHelper.o usbPeripheryAnalyser.o partitionAnalyser.o fileExtensionAnalyser.o virusAnalyser.o \
duckyDetectorGui.o device.o fileHandler.o interface.o setup.o printingService.o printingResultService.o printingDetailsService.o
OBJECTS_PATH := $(patsubst %, $(buildDir)/%, $(OBJECTS))
INCLUDES := -lusb-1.0 -lclamav -lpthread `pkg-config gtkmm-3.0 --cflags --libs`

all: duckyDetector

duckyDetector: src/Detector.cpp $(configDir)/Settings.hpp $(OBJECTS_PATH)
	$(CXX) $(CXX_FLAGS) -o DuckyDetector src/Detector.cpp \
	$(configDir)/Settings.hpp \
	$(appDir)/DuckyDetectorApplicationGui.cpp \
	$(appDir)/DuckyDetectorApplicationTerminal.cpp \
	$(analysisDir)/Scanner.cpp \
	$(analysisDir)/preparation/PreparationHelper.cpp \
	$(analysisDir)/usbPeriphery/UsbPeripheryAnalyser.cpp \
	$(analysisDir)/partition/PartitionAnalyser.cpp \
	$(analysisDir)/fileExtension/FileExtensionAnalyser.cpp \
	$(analysisDir)/virus/MalwareAnalyserWithClamAv.cpp \
	$(guiDir)/DuckyDetectorGui.cpp \
	$(utilDir)/Device.cpp \
	$(utilDir)/FileHandler.cpp \
	$(utilDir)/Interface.cpp \
	$(utilDir)/Setup.cpp \
	$(printDir)/PrintingService.cpp \
	$(printDir)/PrintingResultService.cpp \
	$(printDir)/PrintingDetailsService.cpp \
	$(INCLUDES)

# Application
$(buildDir)/duckyDetectorApplicationGui.o: $(appDir)/DuckyDetectorApplicationGui.hpp $(appDir)/DuckyDetectorApplicationGui.cpp
	$(CXX) $(CXX_FLAGS) -c $< `pkg-config gtkmm-3.0 --cflags --libs`
$(buildDir)/duckyDetectorApplicationTerminal.o: $(appDir)/DuckyDetectorApplicationTerminal.hpp $(appDir)/DuckyDetectorApplicationTerminal.cpp
	$(CXX) $(CXX_FLAGS) -c $<
# Scanner
$(buildDir)/scanner.o: $(analysisDir)/Scanner.hpp $(analysisDir)/Scanner.cpp
	$(CXX) $(CXX_FLAGS) -c $<
$(buildDir)/preparationHelper.o: $(analysisDir)/preparation/PreparationHelper.hpp $(analysisDir)/preparation/PreparationHelper.cpp
	$(CXX) $(CXX_FLAGS) -c $<
$(buildDir)/usbPeripheryAnalyser.o: $(analysisDir)/usbPeriphery/UsbPeripheryAnalyser.hpp $(analysisDir)/usbPeriphery/UsbPeripheryAnalyser.cpp
	$(CXX) $(CXX_FLAGS) -c $<
$(buildDir)/partitionAnalyser.o: $(analysisDir)/partition/PartitionAnalyser.hpp $(analysisDir)/partition/PartitionAnalyser.cpp
	$(CXX) $(CXX_FLAGS) -c $<
$(buildDir)/fileExtensionAnalyser.o: $(analysisDir)/fileExtension/FileExtensionAnalyser.hpp $(analysisDir)/fileExtension/FileExtensionAnalyser.cpp
	$(CXX) $(CXX_FLAGS) -c $<
$(buildDir)/virusAnalyser.o: $(analysisDir)/virus/MalwareAnalyserWithClamAv.hpp $(analysisDir)/virus/MalwareAnalyserWithClamAv.cpp
	$(CXX) $(CXX_FLAGS) -c $<
# GUI
$(buildDir)/duckyDetectorGui.o: $(guiDir)/DuckyDetectorGui.hpp $(guiDir)/DuckyDetectorGui.cpp
	$(CXX) $(CXX_FLAGS) -c $< `pkg-config gtkmm-3.0 --cflags --libs`
# Utils
$(buildDir)/device.o: $(utilDir)/Device.hpp $(utilDir)/Device.cpp
	$(CXX) $(CXX_FLAGS) -c $<
$(buildDir)/fileHandler.o: $(utilDir)/FileHandler.hpp $(utilDir)/FileHandler.cpp
	$(CXX) $(CXX_FLAGS) -c $<
$(buildDir)/interface.o: $(utilDir)/Interface.hpp $(utilDir)/Interface.cpp
	$(CXX) $(CXX_FLAGS) -c $<
$(buildDir)/setup.o: $(utilDir)/Setup.hpp $(utilDir)/Setup.cpp
	$(CXX) $(CXX_FLAGS) -c $<
$(buildDir)/printingService.o: $(printDir)/PrintingService.hpp $(printDir)/PrintingService.cpp
	$(CXX) $(CXX_FLAGS) -c $<
$(buildDir)/printingResultService.o: $(printDir)/PrintingResultService.hpp $(printDir)/PrintingResultService.cpp
	$(CXX) $(CXX_FLAGS) -c $<
$(buildDir)/printingDetailsService.o: $(printDir)/PrintingDetailsService.hpp $(printDir)/PrintingDetailsService.cpp
	$(CXX) $(CXX_FLAGS) -c $<

clean:
	rm *.o DuckyDetector