//
// Created by benji on 30.06.17.
//

#ifndef GRAPHICS_ENGINE_GUI_GUI_H
#define GRAPHICS_ENGINE_GUI_GUI_H
#include <cmath>
#include <iostream>
#include <locale>
#include <vector>
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
#include "FL/Fl_Check_Button.H"

class Editor : public Fl_Group{
protected:
    int infoWidth;
    int infoHeight;
    int editWidth;
    int editHeight;
    int origX, origY;
public:
    Editor(int X, int Y, int W, int H, const char* l);

    Fl_Box* displayBox = NULL;

    Fl_Scroll* scrollInfo = NULL;

    virtual int handle(int event);
};

class FigureEditor : public Editor{
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
        return 11;
    }
public:
    ColorFigureEditor(int X, int Y, int W, int H, const char *l);
};

class LightEditor: public Editor{
private:
    static void changeTypeCB(Fl_Widget *w);
public:
    LightEditor(int X, int Y, int W, int H, const char *l);
};

class GUI : public Fl_Double_Window{
private:
    std::vector<FigureEditor*> figureEditors={};
public:
    friend class FigureEditor;
    GUI(int W, int H, const char *l = 0);
};

#endif //GRAPHICS_ENGINE_GUI_GUI_H
