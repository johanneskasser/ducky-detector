#include "Interface.hpp"

Interface::Interface() = default;

void Interface::addInterface(int pbInterfaceClass, int pbNumEndpoints) {
	this->bInterfaceClass = pbInterfaceClass;
	this->bNumEndpoints = pbNumEndpoints;
}

// compares if a given device already exists
bool Interface::compareInterface(int pbInterfaceClass, int pbNumEndpoints) const {
    // are some parameters seem to be different
    return this->bInterfaceClass == pbInterfaceClass && this->bNumEndpoints == pbNumEndpoints;
}

int Interface::getbInterfaceClass() const {
	return bInterfaceClass;
}

int Interface::getbNumEndpoints() const {
	return bNumEndpoints;
}

Interface::~Interface() = default;
