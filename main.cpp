#include <iostream>
#include "GUI.h"
int main() {
    GUI* gui = new GUI(1920,1080);
    return Fl::run();
}