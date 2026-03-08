#include "headers/app/application.h"

#include <windows.h>

int main() {
    SetConsoleOutputCP(CP_UTF8);
    Application app;
    app.bootstrapDefaults();
    app.run();
    return 0;
}
