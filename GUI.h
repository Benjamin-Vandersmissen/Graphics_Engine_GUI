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
#include <algorithm>
#include <pthread.h>
#include <sys/stat.h>
#include "ini_configuration.hh"
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
#include "FL/Fl_Radio_Round_Button.H"
#include "FL/Fl_BMP_Image.H"
class LightFigureEditor;
class ColorFigureEditor;

class Editor : public Fl_Group{
protected:
    int infoWidth;
    int infoHeight;
    int editWidth;
    int editHeight;
    int origX, origY;
    std::vector<Fl_Input*> inputs;
    std::vector<Fl_Widget*> otherWidgets;
    bool mouseOverInfo(int mouseX, int mouseY);
    bool mouseOverEdit(int mouseX, int mouseY);
public:
    friend LightFigureEditor* colorToLight(ColorFigureEditor* colorFigureEditor);

    friend ColorFigureEditor* lightToColor(LightFigureEditor* lightFigureEditor);

    friend class GUI;

    Editor(int X, int Y, int W, int H, const char* l);

    Fl_Box* displayBox = NULL;

    Fl_Scroll* scrollInfo = NULL;

    virtual int handle(int event);

    virtual std::map<std::string, std::string> getDump();

    void rename();
};

class GenericEditor : public Editor{
private:
    static void deleteCB(Fl_Widget *w, void *v);
    static void renameCB(Fl_Widget* w, void* v);
protected:
    int actionHeight;
    bool mouseOverAction(int mouseX, int mouseY);
public:
    GenericEditor(int X, int Y, int W, int H, const char* l);

    Fl_Group* actionGroup = NULL;

    virtual int handle(int event);

    virtual std::string type() = 0;
};

class FigureEditor : public GenericEditor{
private:
    static void changeTypeCB(Fl_Widget* w, void* v);
public:
    FigureEditor(int X, int Y, int W, int H, const char* l = 0);

    int handle(int event);

    virtual std::map<std::string, std::string> getDump();

    virtual std::string type() = 0;
};

class LightFigureEditor : public FigureEditor{
protected:
public:
    LightFigureEditor(int X, int Y, int W, int H, const char *l);

    std::map<std::string, std::string> getDump();

    std::string type(){
        return "LightFigure";
    }
};


class ColorFigureEditor : public FigureEditor{
protected:

public:
    ColorFigureEditor(int X, int Y, int W, int H, const char *l);

    std::map<std::string, std::string> getDump();

    std::string type(){
        return "ColorFigure";
    }
};

class LightEditor: public GenericEditor{
private:
    static void changeTypeCB(Fl_Widget *w);
public:
    LightEditor(int X, int Y, int W, int H, const char *l);

    std::map<std::string, std::string> getDump();

    int handle(int event);

    std::string type(){
        return "Light";
    }
};

class ImageEditor: public Editor{
private:
    static void changeTypeCB(Fl_Widget* w);

    static void ShadowsCheckCB(Fl_Widget* w);

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

    friend class ImageEditor;

    friend class GenericEditor;

    Fl_Menu_Bar* menuBar = NULL;

    Fl_Scroll* figureEditorsScroll = NULL;

    GUI(int W, int H, const char *l = 0);

    void addFigureEditor(int lightType, int figureType = 0);

    void addLightEditor(int lightType);

    static void addCB(Fl_Widget* w, void* v);

    static void submitCB(Fl_Widget *w, void *v);

    void generateIni();

    static void* generateImage(void* v);

    void clear();

    void loadFromIni(std::string filename);

    FigureEditor* figureEditorFromIni(ini::Section section);

    LightEditor* lightEditorFromIni(ini::Section section);
};

class LightingGroup : public Fl_Group{
private:
    static void colorCB(Fl_Widget* w);
public:
    LightingGroup(int X, int Y, const char* label);

    std::string value();

    void value(double r, double g, double b);

    double r();

    double g();

    double b();
};

void executeCommand(std::string s);

LightFigureEditor* colorToLight(ColorFigureEditor* colorFigureEditor);

ColorFigureEditor* lightToColor(LightFigureEditor* lightFigureEditor);

std::string dtos(double d); //Converts a double to string and removes all useless zeroes

void imagePreviewTimeOut(void* v);

bool fileExists(std::string filename);

bool directoryExists(std::string dirname);
#endif //GRAPHICS_ENGINE_GUI_GUI_H
