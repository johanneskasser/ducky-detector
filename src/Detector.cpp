#include "application/DuckyDetectorApplicationGui.hpp"
#include "application/DuckyDetectorApplicationTerminal.hpp"

int runDuckyDetectorApplicationWithWindow(int argc, char **argv);
int runDuckyDetectorApplicationWithTerminal();

int main(int argc, char* argv[]) {
    if (argc == 1) {
        runDuckyDetectorApplicationWithWindow(argc, argv);
    } else if (argc == 2) {
        std::string arg = argv[1];
        if (arg == "-t") {
            runDuckyDetectorApplicationWithTerminal();
        }
    }

    execl("/sbin/shutdown", "shutdown", "-P", "now", (char*)0);

    return 0;
}

int runDuckyDetectorApplicationWithWindow(int argc, char **argv) {
    auto app = DuckyDetectorApplicationGui::create();
    return app->run(argc, argv);
}

int runDuckyDetectorApplicationWithTerminal() {
    DuckyDetectorApplicationTerminal app;
    return app.run();
}