//
// Created by benji on 30.06.17.
//

#ifndef GRAPHICS_ENGINE_GUI_GUI_H
#define GRAPHICS_ENGINE_GUI_GUI_H
#include <cmath>
#include <iostream>
#include <locale>
#include <vector>
#include <map>
#include <fstream>
#include <exception>
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
#include "FL/Fl_Menu_Bar.H"

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

    virtual std::map<std::string, std::string> getDump();
};

class FigureEditor : public Editor{
protected:
    virtual int defaultWidgets(){
        return 10;
    }

    static void changeTypeCB(Fl_Widget* w, void* v);
public:
    FigureEditor(int X, int Y, int W, int H, const char* l = 0);

    int handle(int event);

    virtual std::map<std::string, std::string> getDump();
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

    std::map<std::string, std::string> getDump();
};

class LightEditor: public Editor{
private:
    static void changeTypeCB(Fl_Widget *w);
public:
    LightEditor(int X, int Y, int W, int H, const char *l);

    std::map<std::string, std::string> getDump();

    int handle(int event);
};

class ImageEditor: public Editor{
public:
    ImageEditor(int X, int Y, int W, int H, const char* l);

    std::map<std::string, std::string> getDump();

};

class GUI : public Fl_Double_Window{
private:
    std::vector<FigureEditor*> figureEditors={};

    std::vector<LightEditor*> lightEditors = {};

    const int STARTX = 100;

    const int STARTY = 100;

    const int EDITORWIDTH = 200;

    const int EDITORHEIGHT = 50;

    const int SEPERATION = 50;
public:
    ImageEditor* imageEditor = NULL;

    friend class FigureEditor;

    friend class LightEditor;

    Fl_Menu_Bar* menuBar = NULL;

    GUI(int W, int H, const char *l = 0);

    void addFigureEditor(int lightType, int figureType = 0);

    void addLightEditor(int lightType);

    static void addCB(Fl_Widget* w, void* v);

    static void submitCB(Fl_Widget *w, void *v);

    void generateIni();
};

void executeCommand(std::string s);

LightFigureEditor* colorToLight(ColorFigureEditor* colorFigureEditor);
#endif //GRAPHICS_ENGINE_GUI_GUI_H
