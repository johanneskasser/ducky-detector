#ifndef INTERFACE_H // prevents multiple compiles
#define INTERFACE_H

class Interface {
    public:
        Interface();
        virtual ~Interface();

        void addInterface(int bInterfaceClass, int bNumEndpoints);
        bool compareInterface(int bInterfaceClass, int bNumEndpoints) const;
        int getbInterfaceClass() const;
        int getbNumEndpoints() const;

    private:
        int bInterfaceClass{};
        int bNumEndpoints{};
};

#endif