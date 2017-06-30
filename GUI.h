//
// Created by benji on 30.06.17.
//

#ifndef GRAPHICS_ENGINE_GUI_GUI_H
#define GRAPHICS_ENGINE_GUI_GUI_H
#include <cmath>
#include <iostream>
#include <locale>
#include "FL/Fl.H"
#include "FL/names.h"
#include "FL/Fl_Group.H"
#include "FL/Fl_Double_Window.H"
#include "FL/Fl_Box.H"
#include "FL/Fl_Scroll.H"
#include "FL/Fl_Input.H"
#include "FL/Fl_Color_Chooser.H"
#include "FL/fl_ask.H"
#include "FL/Fl_Choice.H"

class GUI : public Fl_Double_Window{
public:
    GUI(int W, int H, const char *l = 0);
};

class FigureEditor : public Fl_Group{
protected:
    int infoWidth;
    int infoHeight;
    int editWidth;
    int editHeight;
    virtual int defaultWidgets(){
        return 10;
    }

    static void changeTypeCB(Fl_Widget* w, void* v);
public:
    FigureEditor(int X, int Y, int W, int H, const char* l = 0);

    Fl_Box* displayBox = NULL;

    Fl_Scroll* scrollInfo = NULL;

    int handle(int event);
};

class LightFigureEditor : public FigureEditor{
protected:
    int defaultWidgets(){
        return 19;
    }
public:
    LightFigureEditor(int X, int Y, int W, int H, const char *l);
};


class ColorFigureEditor : public FigureEditor{
protected:
    int defaultWidgets(){
        return 11;}
public:
    ColorFigureEditor(int X, int Y, int W, int H, const char *l);
};

#endif //GRAPHICS_ENGINE_GUI_GUI_H
