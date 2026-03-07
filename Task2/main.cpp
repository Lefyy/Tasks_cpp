#include "headers/app/application.h"

int main() {
    Application app;
    app.bootstrapDefaults();
    app.run();
    return 0;
}
