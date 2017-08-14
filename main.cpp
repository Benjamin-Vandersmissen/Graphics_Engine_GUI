#include <iostream>
#include "GUI.h"
#include "FL/Fl_Shared_Image.H"
int main() {
    fl_register_images();
    GUI* gui = new GUI(1920,1080);
    return Fl::run();
}