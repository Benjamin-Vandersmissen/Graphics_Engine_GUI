//
// Created by benji on 30.06.17.
//

#include <FL/Fl_Group.H>
#include "GUI.h"


GUI::GUI(int W, int H, const char *l) : Fl_Double_Window(W, H, l) {

    this->imageEditor = new ImageEditor(STARTX,STARTY,EDITORWIDTH,EDITORHEIGHT,"Image");

    this->addFigureEditor(1, 0);
    this->addFigureEditor(0, 0);
    this->addLightEditor(0);
    this->addLightEditor(1);
    this->menuBar = new Fl_Menu_Bar(0,0,w(), 32);
    this->menuBar->add("Add a new Figure/Cube", FL_ALT+ '0', addCB, new int(0));
    this->menuBar->add("Add a new Figure/Tetrahedron", FL_ALT+ '1', addCB, new int(1));
    this->menuBar->add("Add a new Figure/Octahedron", FL_ALT+ '2', addCB, new int(2));
    this->menuBar->add("Add a new Figure/Icosahedron", FL_ALT+ '3', addCB, new int(3));
    this->menuBar->add("Add a new Figure/Dodecahedron", FL_ALT+ '4', addCB, new int(4));
    this->menuBar->add("Add a new Figure/Sphere", FL_ALT+ '5', addCB, new int(5));
    this->menuBar->add("Add a new Figure/Cone", FL_ALT+ '6', addCB, new int(6));
    this->menuBar->add("Add a new Figure/Torus", FL_ALT+ '7', addCB, new int(7));

    this->menuBar->add("Add a new Light/Point", FL_ALT + '8', addCB, new int(8));
    this->menuBar->add("Add a new Light/Infinite", FL_ALT + '9', addCB, new int(9));

    this->menuBar->add("Submit", FL_CTRL, submitCB);
    new Fl_Radio_Light_Button(900,100,20,20,"TEST");
    new Fl_Radio_Light_Button(940,100,20,20,"TEST1");
    new Fl_Radio_Light_Button(980,100,20,20,"TEST2");
    new LightingGroup(900,500,"Ambient");
    this->end();
    this->show();
}

Editor::Editor(int X, int Y, int W, int H, const char *l) : Fl_Group(X, Y, W, H), infoWidth(W), infoHeight(H), origX(X), origY(Y) {
    editWidth = 300;
    editHeight= h()+300;
    resizable(NULL); //child widgets won't resize when resizing the group
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
        otherWidgets.push_back((Fl_Widget*)choice);

        Fl_Input* i = new Fl_Input(editX+30, y()+70, 60, 32, "CenterX");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("0");
        inputs.push_back(i);
        i = new Fl_Input(editX+120, y()+70, 60, 32, "CenterY");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("0");
        inputs.push_back(i);
        i = new Fl_Input(editX+210, y()+70, 60, 32, "CenterZ");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("0");
        inputs.push_back(i);
        i = new Fl_Input(editX+30, y()+120, 60, 32, "RotateX");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("0");
        inputs.push_back(i);
        i = new Fl_Input(editX+120, y()+120, 60, 32, "RotateY");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("0");
        inputs.push_back(i);
        i = new Fl_Input(editX+210, y()+120, 60, 32, "RotateZ");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("0");
        inputs.push_back(i);
        i = new Fl_Input(editX+120, y()+170, 60, 32, "Scale");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("1");
        inputs.push_back(i);
        i = new Fl_Input(editX+30, y()+400,60,32, "Detail(\"n\")");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("0");
        i->hide();
        inputs.push_back(i);
        i = new Fl_Input(editX+120, y()+400,60,32, "Height");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("1");
        i->hide();
        inputs.push_back(i);
        i = new Fl_Input(editX+120, y()+400,60,32, "Detail(\"m\")");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("10");
        i->hide();
        inputs.push_back(i);
        i = new Fl_Input(editX+30, y()+450,60,32, "Radius(\"r\")");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("1");
        i->hide();
        inputs.push_back(i);
        i = new Fl_Input(editX+120, y()+450,60,32, "Radius(\"R\")");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("1");
        i->hide();
        inputs.push_back(i);
    }
    this->scrollInfo->end();
    this->end();
}

int Editor::handle(int event) {
    static int offset[2] = {0,0};
    switch(event){
        case FL_ENTER:
            size(editWidth+infoWidth, editHeight);
            if (Fl::belowmouse() != this && Fl::belowmouse() != 0) // Er is al een widget waarover wordt gegaan
                return 0;
            if (Fl::event_x() <= x()+infoWidth && Fl::event_y() <= y()+infoHeight){
                this->scrollInfo->show();
                this->window()->redraw();
            }
            return 1;
        case FL_MOVE:
            if (Fl::event_x() <= x()+infoWidth){
                if (Fl::event_y() <= y()+infoHeight && !this->scrollInfo->visible()) {
                    this->scrollInfo->show();
                    return Fl_Group::handle(event);
                }
                if(Fl::event_y() > y() + infoHeight) {
                    this->scrollInfo->hide();
                }
            }

            return 1;

        case FL_LEAVE:
            size(infoWidth, infoHeight);
            if (this->scrollInfo->visible())
                this->scrollInfo->hide();
            return Fl_Group::handle(event);

        case FL_PUSH:{
            if (Fl::event_x() <= x()+infoWidth && Fl::event_y() <= y()+infoHeight){
                if (Fl::event_clicks() == 1){ //double click on the info to change the name
                    std::map<std::string, std::string> dump = getDump();
                    for(std::pair<std::string, std::string> pair : dump){
                        std::cerr << pair.first << " = " << pair.second << std::endl;
                    }
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
                    this->window()->insert(*this, window()->children());
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

        case FL_RELEASE:{
            this->position(origX, origY);
            this->window()->redraw();
            break;
        }
        default:
            return Fl_Group::handle(event);
    }
    return 0;
}

std::map<std::string, std::string> Editor::getDump() {
    std::map<std::string, std::string> dump = {};
    for(Fl_Input* i : inputs){
        dump[i->label()] = i->value();
    }
    return dump;
}

void FigureEditor::changeTypeCB(Fl_Widget *w, void *v) {
    Fl_Choice* choice = (Fl_Choice*) w;
    FigureEditor* editor = (FigureEditor*) v;
    std::string type = choice->text();
    for(int i = 7; i <= 11; i++){
        editor->inputs[i]->hide();
    }
    if (type == "Sphere"){
        editor->inputs[7]->show();
    }
    else if (type == "Cone" || type == "Cylinder"){
        editor->inputs[7]->show();
        editor->inputs[8]->show();
    }
    else if (type == "Torus"){
        editor->inputs[7]->show();
        editor->inputs[9]->show();
        editor->inputs[10]->show();
        editor->inputs[11]->show();
    }
}

int FigureEditor::handle(int event) {
    switch (event) {
        case FL_RELEASE:{
            GUI* gui = (GUI*) window();
            for(FigureEditor* editor: gui->figureEditors){
                if(editor == this)
                    continue;
                bool condition = Fl::event_x() > editor->x() && Fl::event_x() < editor->x() + editor->infoWidth && Fl::event_y() > editor->y() && Fl::event_y() < editor->y() + editor->infoHeight;
                if (condition) {
                    int tempX, tempY;
                    tempX = editor->origX;
                    tempY = editor->origY;
                    editor->origX = this->origX;
                    editor->origY = this->origY;
                    this->origX = tempX;
                    this->origY = tempY;
                    editor->position(editor->origX, editor->origY);
                }
                this->position(origX,origY);
                gui->redraw();
                break;
            }
        }
        default:
            return Editor::handle(event);
    }
}

std::map<std::string, std::string> FigureEditor::getDump() {
    std::map<std::string, std::string> dump = Editor::getDump();
    Fl_Choice* choice = (Fl_Choice*) otherWidgets[0];
    dump[choice->label()] = choice->text();
    return dump;
}

LightFigureEditor::LightFigureEditor(int X, int Y, int W, int H, const char *l) : FigureEditor(X, Y, W, H, l) {
    this->scrollInfo->begin();
    int editX = x()+infoWidth;
    otherWidgets.push_back(new LightingGroup(editX+80,y()+220,"Ambient"));
    otherWidgets.push_back(new LightingGroup(editX+80,y()+270,"Diffuse"));
    otherWidgets.push_back(new LightingGroup(editX+80,y()+320,"Specular"));
    this->scrollInfo->end();
    this->end();
}

std::map<std::string, std::string> LightFigureEditor::getDump() {
    std::map<std::string ,std::string> dump = FigureEditor::getDump();
    LightingGroup* lightingGroup = (LightingGroup*) otherWidgets[1];
    dump[lightingGroup->label()] = lightingGroup->value();
    lightingGroup = (LightingGroup*) otherWidgets[2];
    dump[lightingGroup->label()] = lightingGroup->value();
    lightingGroup = (LightingGroup*) otherWidgets[3];
    dump[lightingGroup->label()] = lightingGroup->value();
    return dump;
}

ColorFigureEditor::ColorFigureEditor(int X, int Y, int W, int H, const char *l) : FigureEditor(X, Y, W, H, l) {
    int editX = x()+infoWidth;
    Fl_Color_Chooser* chooser = new Fl_Color_Chooser(editX+30,y()+220, 240,150, "Color");
    otherWidgets.push_back((Fl_Widget*)chooser);
    this->scrollInfo->add(chooser);
}

std::map<std::string, std::string> ColorFigureEditor::getDump() {
    std::map<std::string, std::string> dump = FigureEditor::getDump();
    Fl_Color_Chooser* chooser = (Fl_Color_Chooser*) otherWidgets[1];
    dump[chooser->label()] = "(" + std::to_string(chooser->r()) + ", " + std::to_string(chooser->g()) + ", " + std::to_string(chooser->b()) + ")";
    return dump;
}

LightEditor::LightEditor(int X, int Y, int W, int H, const char *l) : Editor(X, Y, W, H, l){
    int editX = x()+infoWidth;
    this->scrollInfo->begin();
    Fl_Check_Button* check = new Fl_Check_Button(editX+120, y()+20, 32,32,"Infinity");
    check->callback(changeTypeCB);
    check->when(FL_WHEN_CHANGED);
    otherWidgets.push_back(check);
    Fl_Input* inp = new Fl_Input(editX+30, y()+70, 60, 32, "X");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    inputs.push_back(inp);
    inp = new Fl_Input(editX+120, y()+70, 60, 32, "Y");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    inputs.push_back(inp);
    inp = new Fl_Input(editX+210, y()+70, 60, 32, "Z");
    inp->align(FL_ALIGN_TOP_LEFT);
    inp->value("0");
    inputs.push_back(inp);
    otherWidgets.push_back(new LightingGroup(editX+80,y()+120,"Ambient"));
    otherWidgets.push_back(new LightingGroup(editX+80,y()+170,"Diffuse"));
    otherWidgets.push_back(new LightingGroup(editX+80,y()+220,"Specular"));
    this->scrollInfo->end();
    this->end();
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

std::map<std::string, std::string> LightEditor::getDump() {
    std::map<std::string, std::string> dump = Editor::getDump();
    Fl_Check_Button* check = (Fl_Check_Button*) otherWidgets[0];
    std::string infinity = (check->value() ? "TRUE" : "FALSE");
    dump["infinity"] = infinity;
    LightingGroup* lightingGroup = (LightingGroup*) otherWidgets[1];
    dump[lightingGroup->label()] = lightingGroup->value();
    lightingGroup = (LightingGroup*) otherWidgets[2];
    dump[lightingGroup->label()] = lightingGroup->value();
    lightingGroup = (LightingGroup*) otherWidgets[3];
    dump[lightingGroup->label()] = lightingGroup->value();
    return dump;
}

int LightEditor::handle(int event) {
    switch (event) {
        case FL_RELEASE:{
            GUI* gui = (GUI*) window();
            for(LightEditor* editor: gui->lightEditors){
                if(editor == this)
                    continue;
                bool condition = Fl::event_x() > editor->x() && Fl::event_x() < editor->x() + editor->infoWidth && Fl::event_y() > editor->y() && Fl::event_y() < editor->y() + editor->infoHeight;
                if (condition) {
                    int tempX, tempY;
                    tempX = editor->origX;
                    tempY = editor->origY;
                    editor->origX = this->origX;
                    editor->origY = this->origY;
                    this->origX = tempX;
                    this->origY = tempY;
                    editor->position(editor->origX, editor->origY);
                }
                this->position(origX,origY);
                gui->redraw();
                break;
            }
        }
        default:
            return Editor::handle(event);
    }}

void GUI::addFigureEditor(int lightType, int figureType) {
	int Y = STARTY+EDITORHEIGHT+2*SEPERATION; //er is altijd al een eerste editor, die van de image, met dubbele seperation ertussen
	if (this->figureEditors.size() != 0){
		Y = this->figureEditors.back()->y() + this->figureEditors.back()->displayBox->h() + 50;
	}
	if (lightType == 0){ //type = Color
		ColorFigureEditor* e = new ColorFigureEditor(STARTX, Y, EDITORWIDTH, EDITORHEIGHT, "Figure");
        Fl_Choice * c = (Fl_Choice*) e->scrollInfo->child(2); // type Choice is 3de widget, 0 & 1 zijn scrollbars
        c->value(figureType);
        c->do_callback();
        this->figureEditors.push_back(e);
        this->add(e);
        return;
    }
    else if (lightType == 1){ //type = Light
        LightFigureEditor* e = new LightFigureEditor(STARTX,Y, EDITORWIDTH, EDITORHEIGHT, "Figure");
        Fl_Choice * c = (Fl_Choice*) e->scrollInfo->child(2);// type Choice is 3de widget, 0 & 1 zijn scrollbars
        c->value(figureType);
        c->do_callback();
        this->figureEditors.push_back(e);
        this->add(e);
    }

}

void GUI::addCB(Fl_Widget *w, void *v) {
    GUI* gui = (GUI*) w->window();
    int i = *(int*) v;
    if (i < 8)
        gui->addFigureEditor(0, i);
    else
        gui->addLightEditor(i-8);
    gui->redraw();
}

ImageEditor::ImageEditor(int X, int Y, int W, int H, const char *l) : Editor(X, Y, W, H, l) {
    int editX = x()+ infoWidth;
    this->displayBox->color(FL_GREEN);
    this->scrollInfo->begin();
    {
        Fl_Choice* choice = new Fl_Choice(editX+60,y()+20,180,32, "Type:");
        choice->add("Wireframe");
        choice->add("ZBufferedWireframe");
        choice->add("ZBuffering");
        choice->add("LightedZBuffering");
        choice->value(0);
        otherWidgets.push_back(choice);

        Fl_Input* input = new Fl_Input(editX + 30, y() + 70, 60, 32, "Size" );
        input->align(FL_ALIGN_TOP_LEFT);
        input->value("0");
        inputs.push_back(input);

        input = new Fl_Input(editX + 30, y() + 120, 60, 32, "EyeX" );
        input->align(FL_ALIGN_TOP_LEFT);
        input->value("0");
        inputs.push_back(input);

        input = new Fl_Input(editX + 120, y() + 120, 60, 32, "EyeY" );
        input->align(FL_ALIGN_TOP_LEFT);
        input->value("0");
        inputs.push_back(input);

        input = new Fl_Input(editX + 210, y() + 120, 60, 32, "EyeZ" );
        input->align(FL_ALIGN_TOP_LEFT);
        input->value("0");
        inputs.push_back(input);

        otherWidgets.push_back(new Fl_Color_Chooser(editX+30,y()+170, 240,150, "BackgroundColor"));
    }
    this->scrollInfo->end();
    this->end();
}

std::map<std::string, std::string> ImageEditor::getDump() {
    std::map<std::string, std::string> dump = Editor::getDump();
    Fl_Choice* choice = (Fl_Choice*) otherWidgets[0];
    dump[choice->label()] = choice->text();
    Fl_Color_Chooser *chooser = (Fl_Color_Chooser *) otherWidgets[1];
    dump[chooser->label()] = "(" + std::to_string(chooser->r()) + ", " + std::to_string(chooser->g()) + ", " + std::to_string(chooser->b()) + ")";
    return dump;
}

void GUI::generateIni() {
    std::string filename = imageEditor->displayBox->label();
    std::ofstream file(filename+".ini");
    file << "[General]" << std::endl;
    std::map<std::string, std::string> imageDump = imageEditor->getDump();
    file << "size = " << imageDump["Size"] << std::endl;
    file << "backgroundcolor = " << imageDump["BackgroundColor"] << std::endl;
    file << "type = \"" << imageDump["Type:"] << "\"" << std::endl;
    std::string eye = "(" + imageDump["EyeX"] + "," + imageDump["EyeY"] + "," + imageDump["EyeZ"] + ")";
    file << "eye = " << eye << std::endl;
    int figures = this->figureEditors.size();
    file << "nrFigures = " << figures << std::endl;
    if(imageDump["Type:"] == "LightedZBuffering"){
        int lights = this->lightEditors.size();
        file << "nrLights = " << lights << std::endl;
        file << std::endl;
        for(int i = 0; i < lights; i++){
            std::map<std::string, std::string> lightDump = lightEditors[i]->getDump();
            bool infinity = lightDump["infinity"] == "TRUE";
            file << "[Light" << i << "]" << std::endl;
            file << "infinity = " << lightDump["infinity"] << std::endl;
            if (infinity){
                file << "direction = (" << lightDump["DirectionX"] <<"," << lightDump["DirectionY"] << "," << lightDump["DirectionZ"] << ")" << std::endl;
            }
            else{
                file << "location = (" << lightDump["X"] << "," << lightDump["Y"] << "," << lightDump["Z"] << ")" << std::endl;
            }
            file << "ambientLight = " << lightDump["Ambient"] << std::endl;
            file << "diffuseLight = " << lightDump["Diffuse"] << std::endl;
            file << "specularLight = " << lightDump["Specular"] << std::endl;
            file << std::endl;
        }
    }
    else
        file << std::endl;
    for(int i = 0; i < figures; i ++){
        file << "[Figure" << i << "]" << std::endl;
        std::map<std::string, std::string> figureDump = figureEditors[i]->getDump();
        bool usesColors = figureDump.find("Color") != figureDump.end();
        file << "type = \"" << figureDump["Type:"] << '\"' << std::endl;
        if (usesColors)
            file << "color = " << figureDump["Color"] << std::endl;
        else{
            file << "ambientReflection = " << figureDump["Ambient"] << std::endl;
            file << "diffuseReflection = " << figureDump["Diffuse"] << std::endl;
            file << "specularReflection = " << figureDump["Specular"] << std::endl;
        }
        if (figureDump.find("Detail(\"n\")") != figureDump.end())
            file << "n = " << figureDump["Detail(\"n\")"] << std::endl;
        if (figureDump.find("Height") != figureDump.end())
            file << "height = " << figureDump["Height"] << std::endl;
        if (figureDump.find("Detail(\"m\")") != figureDump.end())
            file << "m = " << figureDump["Detail(\"m\")"] << std::endl;
        if (figureDump.find("Radius(\"r\")") != figureDump.end())
            file << "r = " << figureDump["Radius(\"r\")"] << std::endl;
        if (figureDump.find("Radius(\"R\")") != figureDump.end())
            file << "R = " << figureDump["Radius(\"R\")"] << std::endl;
        file << "scale = " << figureDump["Scale"] << std::endl;
        file << "rotateX = " << figureDump["RotateX"] << std::endl;
        file << "rotateY = " << figureDump["RotateY"] << std::endl;
        file << "rotateZ = " << figureDump["RotateZ"] << std::endl;
        std::string center = "(" + figureDump["CenterX"] + "," + figureDump["CenterY"] + "," + figureDump["CenterZ"] +")";
        file << "center = " << center << std::endl;

        file << std::endl;
    }
    file.close();
    executeCommand("./engine "+ filename + ".ini");
}

void GUI::submitCB(Fl_Widget *w, void *v) {
    GUI* gui = (GUI*) w->window();
    gui->generateIni();
}

void GUI::addLightEditor(int lightType) {
    int Y = STARTY+EDITORHEIGHT+2*SEPERATION;
    int X = STARTX+EDITORWIDTH+300+SEPERATION;
    if(lightEditors.size() != 0){
        Y = lightEditors.back()->y() + EDITORHEIGHT+SEPERATION;
    }
    LightEditor* editor = new LightEditor(X,Y,EDITORWIDTH,EDITORHEIGHT,"Light");
    Fl_Check_Button* check = (Fl_Check_Button*)editor->scrollInfo->child(0);
    check->value(lightType);
    check->do_callback();
    this->add(editor);
    this->lightEditors.push_back(editor);
}

void executeCommand(std::string s) {
    system(s.c_str());
}

LightFigureEditor *colorToLight(ColorFigureEditor *colorFigureEditor) {
    LightFigureEditor* lightFigureEditor = new LightFigureEditor(colorFigureEditor->x(), colorFigureEditor->y(), colorFigureEditor->w(), colorFigureEditor->h(), colorFigureEditor->displayBox->label());
    Fl_Choice* typechoice1 = (Fl_Choice*) colorFigureEditor->child(0);
    Fl_Choice* typechoice2 = (Fl_Choice*) lightFigureEditor->child(0);
    typechoice2->value(typechoice1->value());
     return lightFigureEditor;
}

LightingGroup::LightingGroup(int X, int Y, const char *label) : Fl_Group(X,Y, 205,32, label){
    align(FL_ALIGN_LEFT);
    Fl_Input* inp = new Fl_Input(X+5, Y, 40, 32, "R");
    inp->value("0");
    inp->align(FL_ALIGN_TOP);
    inp = new Fl_Input(X+60, Y, 40, 32, "G");
    inp->value("0");
    inp->align(FL_ALIGN_TOP);
    inp = new Fl_Input(X+115, Y, 40, 32, "B");
    inp->value("0");
    inp->align(FL_ALIGN_TOP);
    Fl_Button* button = new Fl_Button(X+170,Y,32,32,"..");
    button->callback(colorCB);
    end();
}

void LightingGroup::colorCB(Fl_Widget *w) {
    LightingGroup* group = (LightingGroup*) w->parent();
    double r,g,b;
    Fl_Input* inputR;
    Fl_Input* inputG;
    Fl_Input* inputB;
    inputR = (Fl_Input*) group->child(0);
    inputG = (Fl_Input*) group->child(1);
    inputB = (Fl_Input*) group->child(2);
    r = std::stod(inputR->value());
    g = std::stod(inputG->value());
    b = std::stod(inputB->value());
    fl_color_chooser("Color",r,g,b);

    std::string stringR, stringG, stringB;
    stringR = std::to_string(r).substr(0,4);
    stringG = std::to_string(g).substr(0,4);
    stringB = std::to_string(b).substr(0,4);
    inputR->value(stringR.c_str());
    inputG->value(stringG.c_str());
    inputB->value(stringB.c_str());

}

std::string LightingGroup::value() {
    Fl_Input* inputR = (Fl_Input*) child(0);
    Fl_Input* inputG = (Fl_Input*) child(1);
    Fl_Input* inputB = (Fl_Input*) child(2);
    std::string val = "(";
    val += inputR->value();
    val += ",";
    val += inputG->value();
    val += ",";
    val += inputB->value();
    val += ")";
    return val;
}
