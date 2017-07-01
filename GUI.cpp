//
// Created by benji on 30.06.17.
//

#include <FL/Fl_Group.H>
#include "GUI.h"

GUI::GUI(int W, int H, const char *l) : Fl_Double_Window(W, H, l) {

    this->figureEditors.push_back(new FigureEditor(100,100,200,50, "CUBE"));
//    this->figureEditors.push_back(new FigureEditor(100,200,200,50, "CUBE #1"));
    this->end();
    this->show();
}

Editor::Editor(int X, int Y, int W, int H, const char *l) : Fl_Group(X, Y, W+300, H+300), infoWidth(W), infoHeight(H) {
    editWidth = w()-infoWidth;
    editHeight= h();
    int editX = x()+infoWidth;
    this->displayBox = new Fl_Box(X,Y,infoWidth,H, l);
    this->displayBox->box(FL_PLASTIC_DOWN_BOX);
    this->scrollInfo = new Fl_Scroll(editX,Y, editWidth, editHeight);
    this->scrollInfo->box(FL_PLASTIC_DOWN_BOX);
    this->scrollInfo->selection_color(FL_WHITE);
    this->scrollInfo->end();
    this->scrollInfo->hide();
}

FigureEditor::FigureEditor(int X, int Y, int W, int H, const char *l) : Editor(X, Y, W, H, l){
    int editX = x()+infoWidth;
    this->scrollInfo->begin();
    {
        Fl_Choice* choice = new Fl_Choice(editX+60,y()+20,120,32, "Type:");
        choice->add("Cube");
        choice->add("Tetrahedron");
        choice->add("Octahedron");
        choice->add("Icosahedron");
        choice->add("Dodecahedron");
        choice->add("Sphere");
        choice->add("Cone");
        choice->add("Torus");
        choice->value(0);
        choice->when(FL_WHEN_CHANGED);
        choice->callback(changeTypeCB, this);

        Fl_Input* i = new Fl_Input(editX+30, y()+70, 60, 32, "CenterX");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("0");
        i = new Fl_Input(editX+120, y()+70, 60, 32, "CenterY");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("0");
        i = new Fl_Input(editX+210, y()+70, 60, 32, "CenterZ");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("0");
        i = new Fl_Input(editX+30, y()+120, 60, 32, "RotateX");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("0");
        i = new Fl_Input(editX+120, y()+120, 60, 32, "RotateY");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("0");
        i = new Fl_Input(editX+210, y()+120, 60, 32, "RotateZ");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("0");
        i = new Fl_Input(editX+120, y()+170, 60, 32, "Scale");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("1");
    }
    this->scrollInfo->end();
    this->end();
}

int Editor::handle(int event) {
    static int offset[2] = {0,0};
    switch(event){
        case FL_ENTER:
            if (Fl::event_x() <= x()+infoWidth && Fl::event_y() <= y()+infoHeight){
                this->scrollInfo->show();
                this->window()->insert(*this, 0);
                this->window()->redraw();
            }
            return 1;
        case FL_MOVE:
            if (Fl::event_x() <= x()+infoWidth){
                if (Fl::event_y() <= y()+infoHeight && !this->scrollInfo->visible())
                    this->scrollInfo->show();
                else if (Fl::event_y() > y()+infoHeight){
                    this->scrollInfo->hide();
                }

            }
            return 0;

        case FL_LEAVE:
            if (this->scrollInfo->visible())
                this->scrollInfo->hide();
            return 0;

        case FL_PUSH:{
            if (Fl::event_x() <= x()+infoWidth && Fl::event_y() <= y()+infoHeight){
                if (Fl::event_clicks() == 1){ //double click on the info to change the name
                    const char* text = fl_input("What is the new name?", this->displayBox->label());
                    if (text != 0){
                        this->displayBox->label(text);
                    }
                    return 0;
                }
                else if (Fl::event_clicks() == 0) {
                    this->scrollInfo->hide();
                    offset[0] = x()-Fl::event_x();
                    offset[1] = y()-Fl::event_y();
                    origX = x();
                    origY = y();
                    return 1;
                }
            }
            return Fl_Group::handle(event);
        }
        case FL_DRAG:{
            this->position(Fl::event_x()+offset[0], Fl::event_y()+offset[1]);
            this->window()->redraw();
            return 1;
        }
        default:
            return Fl_Group::handle(event);
    }
}

void FigureEditor::changeTypeCB(Fl_Widget *w, void *v) {
    Fl_Choice* choice = (Fl_Choice*) w;
    FigureEditor* editor = (FigureEditor*) v;
    std::string type = choice->text();
    int editX = editor->x()+ editor->infoWidth;
    while(editor->scrollInfo->children() != editor->defaultWidgets()){//8 inputfields + 2 (hidden) scrollbars => standaard
        Fl::delete_widget(editor->scrollInfo->child(editor->defaultWidgets()-2));
        editor->scrollInfo->remove(editor->defaultWidgets()-2);
    }

    if (type == "Sphere"){
        Fl_Input* inp = new Fl_Input(editX+30, editor->y()+400,60,32, "Detail(\"n\")");
        inp->align(FL_ALIGN_TOP_LEFT);
        inp->value("0");
        editor->scrollInfo->insert(*inp, editor->defaultWidgets()-2);
    }
    else if (type == "Cone" || type == "Cylinder"){
        Fl_Input* inp = new Fl_Input(editX+30, editor->y()+400,60,32, "Detail(\"n\")");
        inp->align(FL_ALIGN_TOP_LEFT);
        inp->value("10");
        editor->scrollInfo->insert(*inp, editor->defaultWidgets()-2);
        inp = new Fl_Input(editX+120, editor->y()+400,60,32, "Height");
        inp->align(FL_ALIGN_TOP_LEFT);
        inp->value("1");
        editor->scrollInfo->insert(*inp,editor->defaultWidgets()-1);
    }
    else if (type == "Torus"){
        Fl_Input* inp = new Fl_Input(editX+30, editor->y()+400,60,32, "Radius(\"r\")");
        inp->align(FL_ALIGN_TOP_LEFT);
        inp->value("1");
        editor->scrollInfo->insert(*inp, editor->defaultWidgets()-2);
        inp = new Fl_Input(editX+120, editor->y()+400,60,32, "Radius(\"R\")");
        inp->align(FL_ALIGN_TOP_LEFT);
        inp->value("1");
        editor->scrollInfo->insert(*inp,editor->defaultWidgets()-1);
        inp = new Fl_Input(editX+30, editor->y()+450,60,32, "Detail(\"n\")");
        inp->align(FL_ALIGN_TOP_LEFT);
        inp->value("10");
        editor->scrollInfo->insert(*inp, editor->defaultWidgets());
        inp = new Fl_Input(editX+120, editor->y()+450,60,32, "Detail(\"m\")");
        inp->align(FL_ALIGN_TOP_LEFT);
        inp->value("10");
        editor->scrollInfo->insert(*inp,editor->defaultWidgets()+1);
    }
}

int FigureEditor::handle(int event) {
    int e = Editor::handle(event);
    switch (event) {
        case FL_RELEASE:{
            GUI* gui = (GUI*) window();
            for(FigureEditor* editor: gui->figureEditors){
                if(editor == this)
                    continue;
                bool condition = Fl::event_x() > editor->x() && Fl::event_x() < editor->x() + editor->infoWidth && Fl::event_y() > editor->y() && Fl::event_y() < editor->y() + editor->infoHeight;
                if (condition)
                    std::cerr << "A" << std::endl;
            }
        }
        default:
            return e;
    }
}

LightFigureEditor::LightFigureEditor(int X, int Y, int W, int H, const char *l) : FigureEditor(X, Y, W, H, l) {
    int editX = x()+infoWidth;
    Fl_Input* inp = new Fl_Input(editX+30, y()+220, 60,32, "AmbientR");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    this->scrollInfo->insert(*inp,9);
    inp = new Fl_Input(editX+120, y()+220, 60,32, "AmbientG");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    this->scrollInfo->insert(*inp,10);
    inp = new Fl_Input(editX+210, y()+220, 60,32, "AmbientB");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    this->scrollInfo->insert(*inp,11);

    inp = new Fl_Input(editX+30, y()+270, 60,32, "DiffuseR");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    this->scrollInfo->insert(*inp,12);
    inp = new Fl_Input(editX+120, y()+270, 60,32, "DiffuseG");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    this->scrollInfo->insert(*inp,13);
    inp = new Fl_Input(editX+210, y()+270, 60,32, "DiffuseB");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    this->scrollInfo->insert(*inp,14);

    inp = new Fl_Input(editX+30, y()+320, 60,32, "SpecularR");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    this->scrollInfo->insert(*inp,15);
    inp = new Fl_Input(editX+120, y()+320, 60,32, "SpecularG");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    this->scrollInfo->insert(*inp,16);
    inp = new Fl_Input(editX+210, y()+320, 60,32, "SpecularB");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    this->scrollInfo->insert(*inp,17);
}

ColorFigureEditor::ColorFigureEditor(int X, int Y, int W, int H, const char *l) : FigureEditor(X, Y, W, H, l) {
    int editX = x()+infoWidth;
    this->scrollInfo->insert(*(new Fl_Color_Chooser(editX+30,y()+220, 240,150, "Color")),9);
}

LightEditor::LightEditor(int X, int Y, int W, int H, const char *l) : Editor(X, Y, W, H, l){
    int editX = x()+infoWidth;
    this->scrollInfo->begin();
    Fl_Check_Button* check = new Fl_Check_Button(editX+120, y()+20, 32,32,"Infinity");
    check->callback(changeTypeCB);
    check->when(FL_WHEN_CHANGED);
    Fl_Input* inp = new Fl_Input(editX+30, y()+70, 60, 32, "X");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    inp = new Fl_Input(editX+120, y()+70, 60, 32, "Y");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    inp = new Fl_Input(editX+210, y()+70, 60, 32, "Z");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");

    inp = new Fl_Input(editX+30, y()+120, 60, 32, "AmbientR");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    inp = new Fl_Input(editX+120, y()+120, 60, 32, "AmbientG");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    inp = new Fl_Input(editX+210, y()+120, 60, 32, "AmbientB");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");

    inp = new Fl_Input(editX+30, y()+170, 60, 32, "DiffuseR");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    inp = new Fl_Input(editX+120, y()+170, 60, 32, "DiffuseG");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    inp = new Fl_Input(editX+210, y()+170, 60, 32, "DiffuseB");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");

    inp = new Fl_Input(editX+30, y()+220, 60, 32, "SpecularR");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    inp = new Fl_Input(editX+120, y()+220, 60, 32, "SpecularG");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    inp = new Fl_Input(editX+210, y()+220, 60, 32, "SpecularB");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
}

void LightEditor::changeTypeCB(Fl_Widget *w) {
    Fl_Scroll* scroll = (Fl_Scroll*) w->parent();
    Fl_Check_Button* check = (Fl_Check_Button*) w;
    if (check->value()){
        scroll->child(1)->label("DirectionX");
        scroll->child(2)->label("DirectionY");
        scroll->child(3)->label("DirectionZ");
    }
    else{
        scroll->child(1)->label("X");
        scroll->child(2)->label("Y");
        scroll->child(3)->label("Z");
    }
    scroll->redraw();
}

