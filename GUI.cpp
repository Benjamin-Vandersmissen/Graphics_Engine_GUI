//
// Created by benji on 30.06.17.
//

#include "GUI.h"


GUI::GUI(int W, int H, const char *l) : Fl_Double_Window(W, H, l) {

    this->imageEditor = new ImageEditor(STARTX,STARTY,EDITORWIDTH,EDITORHEIGHT,"Image");

    figureEditorsScroll = new Fl_Scroll(100-Fl::scrollbar_size()-5,STARTY+EDITORHEIGHT+SEPERATION, EDITORWIDTH+imageEditor->editWidth+Fl::scrollbar_size_, 800);
    figureEditorsScroll->end();
    figureEditorsScroll->scroll_to(0,0);
    figureEditorsScroll->type(Fl_Scroll::VERTICAL);
    figureEditorsScroll->scrollbar.align(FL_ALIGN_LEFT);
    this->addFigureEditor(0, 0);
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
    new Fl_Radio_Round_Button(900,100,20,20,"TEST");
    new Fl_Radio_Round_Button(940,100,20,20,"TEST1");
    new Fl_Radio_Round_Button(980,100,20,20,"TEST2");
    loadFromIni("Image.ini");
    for(FigureEditor* editor: figureEditors){
        std::cerr << editor->origX << ' ' << editor->origY << std::endl;
    }
    this->end();
    this->show();
}

Editor::Editor(int X, int Y, int W, int H, const char *l) : Fl_Group(X, Y, W, H, l), infoWidth(W), infoHeight(H), origX(X), origY(Y) {
    labeltype(FL_NO_LABEL);
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

FigureEditor::FigureEditor(int X, int Y, int W, int H, const char *l) : GenericEditor(X, Y, W, H, l){
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
        i = new Fl_Input(editX+30, y()+170, 60, 32, "Scale");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("1");
        inputs.push_back(i);
        i = new Fl_Input(editX+120, y()+170, 60, 32, "Reflection");
        i->align(FL_ALIGN_TOP_LEFT);
        i->value("1");
        i->hide();
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
    static int sizes[2] = {0,0};
    GUI* gui = (GUI*) window();
    switch(event){
        case FL_ENTER: {
            std::cerr << origY << ' ' << y() << ' ' << y() + gui->figureEditorsScroll->yposition() << std::endl;
            this->parent()->insert(*this, window()->children());
            size(editWidth + infoWidth, editHeight);
            if (displayBox->y() + editHeight > this->parent()->y() + this->parent()->h()) {
                this->y(displayBox->y() - editHeight + infoHeight);
                this->scrollInfo->position(scrollInfo->x(), y());
            }
            this->scrollInfo->show();
            this->window()->redraw();
            return 1;
        }
        case FL_MOVE: {
            int a = mouseOverInfo(Fl::event_x(), Fl::event_y()) | mouseOverEdit(Fl::event_x(), Fl::event_y()) << 1;
            switch(a){ //over None
                case 0x0:{
                    if (scrollInfo->visible()){
                        scrollInfo->hide();
                    }
                    window()->redraw();
                    size(infoWidth, infoHeight);
                    break;
                }
                case 0x1:{ //over Info
                    if (!scrollInfo->visible()){
                        scrollInfo->show();
                    }
                    break;
                }
                case 0x2:{ //over Edit
                    break;
                }
                default:{// shouldn't be reached
                    break;
                }
            }
            return 1;
        }
        case FL_LEAVE: {
            size(infoWidth, infoHeight);
            y(displayBox->y());
            scrollInfo->position(scrollInfo->x(), y());
            if (this->scrollInfo->visible())
                this->scrollInfo->hide();
            return Fl_Group::handle(event);
        }

        case FL_PUSH:{
            if (mouseOverInfo(Fl::event_x(), Fl::event_y())){
                this->scrollInfo->hide();
                if (Fl::event_clicks() == 1){ //double click on the info to change the name
                    rename();
                    return 0;
                }
                else if (Fl::event_clicks() == 0) {
                    offset[0] = x()-Fl::event_x();
                    offset[1] = y()-Fl::event_y();
                    sizes[0] = w();
                    sizes[1] = h();
                    size(infoWidth, infoHeight);
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
            this->size(sizes[0], sizes[1]);
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
        if (i->visible())
            dump[i->label()] = i->value();
    }
    return dump;
}

bool GenericEditor::mouseOverAction(int mouseX, int mouseY) {
    bool a = mouseX >= actionGroup->x() && mouseX < actionGroup->x()+infoWidth && mouseY >= actionGroup->y() && mouseY < actionGroup->y()+actionHeight;
    return a;
}

GenericEditor::GenericEditor(int X, int Y, int W, int H, const char *l) : Editor(X, Y, W, H, l) {
    actionHeight = 30;
    this->actionGroup = new Fl_Group(X,Y+H,W,actionHeight);
    this->actionGroup->box(FL_PLASTIC_UP_FRAME);
    this->actionGroup->end();
    Fl_Button* deleteButton = new Fl_Button(X+10,Y+infoHeight+5,20,20,"@9+");
    deleteButton->labelcolor(FL_DARK_RED);
    deleteButton->callback(deleteCB, this);
    this->actionGroup->add(deleteButton);
    Fl_Button* renameButton = new Fl_Button(X+40, Y+infoHeight+5, 60, 20, "Rename");
    renameButton->callback(renameCB, this);
    this->actionGroup->add(renameButton);
    this->actionGroup->hide();
    this->add(actionGroup);
}

int GenericEditor::handle(int event) {
    switch (event) {
        case FL_ENTER:{
            if (displayBox->y() + infoHeight+actionHeight > parent()->y()+parent()->h()){
                actionGroup->position(actionGroup->x(), displayBox->y()-actionHeight);
            }
            return Editor::handle(event);
        }
        case FL_PUSH :{
            if (mouseOverInfo(Fl::event_x(), Fl::event_y())){
                actionGroup->hide();
            }
            return Editor::handle(event);
        }
        case FL_MOVE: {
            int a = mouseOverInfo(Fl::event_x(), Fl::event_y()) | mouseOverEdit(Fl::event_x(), Fl::event_y()) << 1 |
                    mouseOverAction(Fl::event_x(), Fl::event_y()) << 2;
            switch (a) {
                case 0x0: { // over None
                    if (scrollInfo->visible()) {
                        scrollInfo->hide();
                    }
                    if (actionGroup->visible()) {
                        actionGroup->hide();
                    }
                    window()->redraw();
                    size(infoWidth, infoHeight);
                    break;
                }
                case 0x1: { //over Info
                    if (!scrollInfo->visible()) {
                        scrollInfo->show();
                    }
                    if (!actionGroup->visible()) {
                        actionGroup->show();
                    }
                    break;
                }
                case 0x2: { //over Edit
                    if (actionGroup->visible()) {
                        actionGroup->hide();
                        window()->redraw();
                    }
                    break;
                }
                case 0x4: { //over Action
                    if (scrollInfo->visible()) {
                        scrollInfo->hide();
                        window()->redraw();
                    }
                    break;
                }
                default: {// shouldn't be reached
                    break;
                }
            }
            return 1;
        }
        case FL_LEAVE:{
            actionGroup->hide();
            actionGroup->position(actionGroup->x(), displayBox->y()+infoHeight);
            return Editor::handle(event);
        }
        default:
            return Editor::handle(event);
    }
}

void GenericEditor::deleteCB(Fl_Widget *w, void *v) {
    GenericEditor* widget = (GenericEditor*) v;
    widget->hide();
    GUI* gui = (GUI*) ((GenericEditor*)v)->window();
    if (widget->type() == "ColorFigure" || widget->type() == "LightFigure") {
        long index = std::distance(gui->figureEditors.begin(),
                                   std::find(gui->figureEditors.begin(), gui->figureEditors.end(), widget));
        if (index == gui->figureEditors.size())
            return;
        for (long a = index + 1; a < gui->figureEditors.size(); a++) { //move all editors under the deleted editor up
            gui->figureEditors[a]->position(gui->figureEditors[a]->x(),
                                            gui->figureEditors[a]->y() - gui->SEPERATION - gui->EDITORHEIGHT);
        }
        gui->figureEditors.erase(std::remove(gui->figureEditors.begin(), gui->figureEditors.end(), widget), gui->figureEditors.end());
    }
    else if (widget->type() == "Light"){
        long index = std::distance(gui->lightEditors.begin(),
                                   std::find(gui->lightEditors.begin(), gui->lightEditors.end(), widget));
        if (index == gui->lightEditors.size())
            return;
        for (long a = index + 1; a < gui->lightEditors.size(); a++) {
            gui->lightEditors[a]->position(gui->lightEditors[a]->x(),
                                            gui->lightEditors[a]->y() - gui->SEPERATION - gui->EDITORHEIGHT);
        }
        gui->lightEditors.erase(std::remove(gui->lightEditors.begin(), gui->lightEditors.end(), widget), gui->lightEditors.end());
    }
    Fl::delete_widget(widget);
}

void GenericEditor::renameCB(Fl_Widget *w, void *v) {
    GenericEditor* widget = (GenericEditor*) v;
    widget->rename();
}


void FigureEditor::changeTypeCB(Fl_Widget *w, void *v) {
    Fl_Choice* choice = (Fl_Choice*) w;
    FigureEditor* editor = (FigureEditor*) v;
    std::string type = choice->text();
    for(int i = 8; i <= 12; i++){
        editor->inputs[i]->hide();
    }
    if (type == "Sphere"){
        editor->inputs[8]->show();
    }
    else if (type == "Cone" || type == "Cylinder"){
        editor->inputs[8]->show();
        editor->inputs[9]->show();
    }
    else if (type == "Torus"){
        editor->inputs[8]->show();
        editor->inputs[10]->show();
        editor->inputs[11]->show();
        editor->inputs[12]->show();
    }
}

int FigureEditor::handle(int event) {
    switch (event) {
        case FL_RELEASE:{
            std::cerr << origY << std::endl;
            GUI* gui = (GUI*) window();
            int dY = 0;
            for(FigureEditor* editor: gui->figureEditors){
                if(editor == this)
                    continue;
                bool condition = Fl::event_x() > editor->x() && Fl::event_x() < editor->x() + editor->infoWidth && Fl::event_y() > editor->y() && Fl::event_y() < editor->y() + editor->infoHeight;
                if (condition) {
                    int tempOrigX, tempOrigY;
                    tempOrigX = editor->origX;
                    tempOrigY = editor->origY;
                    dY = editor->y()-editor->origY;
                    std::cerr << dY << ' ' << gui->figureEditorsScroll->yposition() << std::endl;
                    editor->origX = this->origX;
                    editor->origY = this->origY;
                    this->origX = tempOrigX;
                    this->origY = tempOrigY;
                    editor->position(editor->origX, editor->origY);
                    break;
                }
            }
            this->position(origX,origY);
            gui->redraw();
            return 0;
        }
        default:
            return GenericEditor::handle(event);
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
    inputs[7]->show();
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
    dump[chooser->label()] = "(" + dtos(chooser->r()) + ", " + dtos(chooser->g()) + ", " + dtos(chooser->b()) + ")";
    return dump;
}

LightEditor::LightEditor(int X, int Y, int W, int H, const char *l) : GenericEditor(X, Y, W, H, l){
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
            }
            this->position(origX,origY);
            gui->redraw();
            break;
        }
        default:
            return GenericEditor::handle(event);
    }}

void GUI::addFigureEditor(int lightType, int figureType) {
	int Y = STARTY+EDITORHEIGHT+2*SEPERATION; //er is altijd al een eerste editor, die van de image, met dubbele seperation ertussen
	if (this->figureEditors.size() != 0){
		Y = this->figureEditors.back()->y() + this->figureEditors.back()->displayBox->h() + 50;
	}
	if (lightType == 0){ //type = Color
		ColorFigureEditor* e = new ColorFigureEditor(STARTX, Y, EDITORWIDTH, EDITORHEIGHT, "Figure");
        Fl_Choice * c = (Fl_Choice*) e->otherWidgets[0];
        c->value(figureType);
        c->do_callback();
        this->figureEditors.push_back(e);
        this->figureEditorsScroll->add(e);
        return;
    }
    else if (lightType == 1){ //type = Light
        LightFigureEditor* e = new LightFigureEditor(STARTX,Y, EDITORWIDTH, EDITORHEIGHT, "Figure");
        Fl_Choice * c = (Fl_Choice*) e->otherWidgets[0];
        c->value(figureType);
        c->do_callback();
        this->figureEditors.push_back(e);
        this->figureEditorsScroll->add(e);
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
        choice->when(FL_WHEN_CHANGED);
        choice->callback(changeTypeCB);
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

        input = new Fl_Input(editX + 210, y() + 70, 60, 32, "shadowMask");
        input->align(FL_ALIGN_TOP_LEFT);
        input->value("0");
        input->hide();
        inputs.push_back(input);

        otherWidgets.push_back(new Fl_Color_Chooser(editX+30,y()+170, 240,150, "BackgroundColor"));

        Fl_Check_Button* shadowsCheck = new Fl_Check_Button(editX+170, y() + 70, 17,14, "Shadows");
        shadowsCheck->align(FL_ALIGN_LEFT);
        shadowsCheck->callback(ShadowsCheckCB);
        otherWidgets.push_back(shadowsCheck);
    }
    this->scrollInfo->end();
    this->end();
}

std::map<std::string, std::string> ImageEditor::getDump() {
    std::map<std::string, std::string> dump = Editor::getDump();
    Fl_Choice* choice = (Fl_Choice*) otherWidgets[0];
    dump[choice->label()] = choice->text();
    Fl_Color_Chooser *chooser = (Fl_Color_Chooser *) otherWidgets[1];
    dump[chooser->label()] = "(" + dtos(chooser->r()) + ", " + dtos(chooser->g()) + ", " + dtos(chooser->b()) + ")";
    Fl_Check_Button* check = (Fl_Check_Button*) otherWidgets[2];
    dump[check->label()] = (check->value() ? "TRUE" : "FALSE");
    return dump;
}

void ImageEditor::changeTypeCB(Fl_Widget *w) {
    GUI* gui = (GUI*) w->window();
    ImageEditor* imageEditor = gui->imageEditor;
    Fl_Choice* typechoice = (Fl_Choice*) imageEditor->otherWidgets[0];
    std::string imageType = typechoice->text();
    if (gui->figureEditors[0]->type() == "ColorFigure" && imageType == "LightedZBuffering"){
        for(int i = 0; i < gui->figureEditors.size(); i++){
            LightFigureEditor* lightFigureEditor = colorToLight((ColorFigureEditor*)gui->figureEditors[i]);
            Fl::delete_widget(gui->figureEditors[i]);
            gui->figureEditors[i] = lightFigureEditor;
            gui->figureEditorsScroll->add(lightFigureEditor);
        }
    }
    else if (gui->figureEditors[0]->type() == "LightFigure" && imageType != "LightedZBuffering"){
        for(int i = 0; i < gui->figureEditors.size(); i++){
            ColorFigureEditor* colorFigureEditor = lightToColor((LightFigureEditor*)gui->figureEditors[i]);
            Fl::delete_widget(gui->figureEditors[i]);
            gui->figureEditors[i] = colorFigureEditor;
            gui->figureEditorsScroll->add(colorFigureEditor);
        }
    }
    if (imageType == "LightedZBuffering" && !imageEditor->otherWidgets[2]->visible()){
        imageEditor->otherWidgets[2]->show();
    }
    else if (imageType != "LightedZBuffering" && imageEditor->otherWidgets[2]->visible()){
        imageEditor->otherWidgets[2]->hide();
    }
}

void ImageEditor::ShadowsCheckCB(Fl_Widget *w) {
    ImageEditor* editor = (ImageEditor*) w->parent()->parent(); // checkbox->infoScroll->imageEditor
    Fl_Check_Button* check = (Fl_Check_Button*) w;
    if (check->value()){
        editor->inputs[4]->show();
    }
    else{
        editor->inputs[4]->hide();
    }
}

void GUI::generateIni() {
    std::string filename = imageEditor->label();
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
        file << "shadowEnabled = " << imageDump["Shadows"] << std::endl;
        file << "shadowMask = " << imageDump["shadowMask"] << std::endl;
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
            file << "reflectionCoefficient = " << figureDump["Reflection"] << std::endl;
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
    Fl_Double_Window* win  = new Fl_Double_Window(200,100,imageEditor->label());
    win->end();
    win->show();
    Fl::add_timeout(0.5,imagePreviewTimeOut, win);
    pthread_t thread;
    pthread_create(&thread, NULL, generateImage, this);
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

void GUI::clear() {
    for(FigureEditor* editor : figureEditors){
        Fl::delete_widget(editor);
    }
    for(LightEditor* editor : lightEditors){
        Fl::delete_widget(editor);
    }
    Fl::delete_widget(imageEditor);
    imageEditor = NULL;
    figureEditors= {};
    lightEditors = {};
}

void GUI::loadFromIni(std::string filename) {
    clear();
    std::ifstream ini(filename);
    ini::Configuration config;
    ini >> config;
    int size = config["General"]["size"].as_int_or_die();
    std::vector<double> colors = config["General"]["backgroundcolor"].as_double_tuple_or_die();
    std::string type = config["General"]["type"].as_string_or_die();
    int nrFigures = config["General"]["nrFigures"].as_int_or_die();
    std::vector<double> eye = config["General"]["eye"].as_double_tuple_or_die();
    bool shadowsEnabled = config["General"]["shadowEnabled"].as_bool_or_default(false);
    int shadowMaskSize = config["General"]["shadowMask"].as_int_or_default(0);
    int nrLights = config["General"]["nrLights"].as_int_or_default(0);
    imageEditor = new ImageEditor(STARTX,STARTY,EDITORWIDTH,EDITORHEIGHT,"Image");
    Fl_Choice* choice = (Fl_Choice*)imageEditor->otherWidgets[0];
    choice->value(choice->find_index(type.c_str()));
    choice->do_callback();
    Fl_Color_Chooser* chooser = (Fl_Color_Chooser*) imageEditor->otherWidgets[1];
    chooser->rgb(colors[0],colors[1],colors[2]);
    Fl_Check_Button* check_button = (Fl_Check_Button*) imageEditor->otherWidgets[2];
    check_button->value(shadowsEnabled);
    check_button->do_callback();
    std::vector<std::string> args = {dtos(size), dtos(eye[0]), dtos(eye[1]), dtos(eye[2]), dtos(shadowMaskSize)};
    for(int i = 0; i < args.size(); i++){
        imageEditor->inputs[i]->value(args[i].c_str());
    }
    for(int i = 0; i < nrFigures; i++){
        std::string name = "Figure" + dtos(i);
        FigureEditor* editor = figureEditorFromIni(config[name]);
        int Y = STARTY+EDITORHEIGHT+2*SEPERATION + this->figureEditors.size()*(EDITORHEIGHT+SEPERATION); //er is altijd al een eerste editor, die van de image, met dubbele seperation ertussen
        editor->position(STARTX,Y);
        editor->origX = STARTX;
        editor->origY = Y;
        figureEditors.push_back(editor);
        this->figureEditorsScroll->add(editor);
    }
    for(int i = 0; i < nrLights; i ++){
        std::string name = "Light" + dtos(i);
        LightEditor* editor = lightEditorFromIni(config[name]);
        int Y = STARTY+EDITORHEIGHT+2*SEPERATION + this->lightEditors.size()*(EDITORHEIGHT+SEPERATION);
        editor->position(STARTX+EDITORWIDTH+300+SEPERATION,Y);
        editor->origX = STARTX+EDITORWIDTH+300+SEPERATION;
        editor->origY = Y;
        lightEditors.push_back(editor);
        this->add(editor);
    }
}

FigureEditor *GUI::figureEditorFromIni(ini::Section section) {
    std::string type = section["type"].as_string_or_die();
    double scale = section["scale"].as_double_or_die();
    double rotateX = section["rotateX"].as_double_or_die();
    double rotateY = section["rotateY"].as_double_or_die();
    double rotateZ = section["rotateZ"].as_double_or_die();
    std::vector<double> center = section["center"].as_double_tuple_or_die();
    int n = section["n"].as_int_or_default(0);
    int m = section["m"].as_int_or_default(0);
    double r = section["r"].as_double_or_default(0);
    double R = section["R"].as_double_or_default(0);
    double height = section["height"].as_double_or_default(0);
    double reflection = section["reflectionCoefficient"].as_double_or_default(0);
    std::vector<double> color;
    FigureEditor* editor;
    if (section["color"].as_double_tuple_if_exists(color)){
        editor = new ColorFigureEditor(0,0,EDITORWIDTH, EDITORHEIGHT, "Figure");
        Fl_Color_Chooser* chooser = (Fl_Color_Chooser*) editor->otherWidgets[1];
        chooser->rgb(color[0], color[1], color[2]);
    }
    else{
        editor = new LightFigureEditor(0,0,EDITORWIDTH, EDITORHEIGHT, "Figure");
        std::vector<double> ambient = section["ambientReflection"].as_double_tuple_or_die();
        std::vector<double> diffuse = section["diffuseReflection"].as_double_tuple_or_die();
        std::vector<double> specular = section["specularReflection"].as_double_tuple_or_die();
        LightingGroup* lightingGroup = (LightingGroup*) editor->otherWidgets[1];
        lightingGroup->value(ambient[0], ambient[1], ambient[2]);
        lightingGroup = (LightingGroup*) editor->otherWidgets[2];
        lightingGroup->value(diffuse[0], diffuse[1], diffuse[2]);
        lightingGroup = (LightingGroup*) editor->otherWidgets[3];
        lightingGroup->value(specular[0], specular[1], specular[2]);
    }
    std::vector<std::string> values = {dtos(center[0]), dtos(center[1]), dtos(center[2]),
                                       dtos(rotateX), dtos(rotateY), dtos(rotateZ),
                                       dtos(scale), dtos(reflection),dtos(n),
                                       dtos(height), dtos(m), dtos(r), dtos(R)};
    for(int i = 0; i < values.size(); i++){
        editor->inputs[i]->value(values[i].c_str());
    }
    Fl_Choice* choice = (Fl_Choice*) editor->otherWidgets[0];
    choice->value(choice->find_index(type.c_str()));
    return editor;
}

LightEditor *GUI::lightEditorFromIni(ini::Section section) {
    bool infinity = section["infinity"].as_bool_or_die();
    std::vector<double> ambient = section["ambientLight"].as_double_tuple_or_die();
    std::vector<double> diffuse = section["diffuseLight"].as_double_tuple_or_die();
    std::vector<double> specular = section["specularLight"].as_double_tuple_or_die();
    std::vector<double> locationDirection = section[(infinity? "direction" : "location")].as_double_tuple_or_die();
    LightEditor* editor = new LightEditor(0,0,EDITORWIDTH,EDITORHEIGHT,"Light");
    Fl_Check_Button* check_button = (Fl_Check_Button*) editor->otherWidgets[0];
    check_button->value(infinity);
    LightingGroup* lightingGroup = (LightingGroup*) editor->otherWidgets[1];
    lightingGroup->value(ambient[0], ambient[1], ambient[2]);
    lightingGroup = (LightingGroup*) editor->otherWidgets[2];
    lightingGroup->value(diffuse[0], diffuse[1], diffuse[2]);
    lightingGroup = (LightingGroup*) editor->otherWidgets[3];
    lightingGroup->value(specular[0], specular[1], specular[2]);
    std::vector<std::string> args = {dtos(locationDirection[0]), dtos(locationDirection[1]), dtos(locationDirection[2])};
    for(int i = 0; i < args.size(); i++){
        editor->inputs[i]->value(args[i].c_str());
    }
    return editor;
}

void* GUI::generateImage(void* v) {
    ImageEditor* editor = ((GUI*) v)->imageEditor;
    std::string filename = (std::string)editor->label();
    if (fileExists(filename+".bmp"))
        executeCommand("rm " + filename + ".bmp");
    std::string command = "./engine " + filename + ".ini";
    executeCommand(command);
    pthread_exit(NULL);
}

void executeCommand(std::string s) {
    system(s.c_str());
}

LightFigureEditor *colorToLight(ColorFigureEditor *colorFigureEditor) {
    LightFigureEditor* lightFigureEditor = new LightFigureEditor(colorFigureEditor->x(), colorFigureEditor->y(), colorFigureEditor->infoWidth, colorFigureEditor->infoHeight, colorFigureEditor->label());
    Fl_Choice* typechoice1 = (Fl_Choice*) colorFigureEditor->otherWidgets[0];
    Fl_Choice* typechoice2 = (Fl_Choice*) lightFigureEditor->otherWidgets[0];
    typechoice2->value(typechoice1->value());
    typechoice2->do_callback();
    Fl_Color_Chooser* chooser = (Fl_Color_Chooser*) colorFigureEditor->otherWidgets[1];
    LightingGroup* lightingGroup = (LightingGroup*) lightFigureEditor->otherWidgets[1];
    lightingGroup->value(chooser->r(),chooser->g(),chooser->b());
    for(int i = 0; i < colorFigureEditor->inputs.size();i++){
        Fl_Input* input1 = colorFigureEditor->inputs[i];
        Fl_Input* input2 = lightFigureEditor->inputs[i];
        input2->value(input1->value());
    }
    return lightFigureEditor;
}

ColorFigureEditor *lightToColor(LightFigureEditor *lightFigureEditor) {
    ColorFigureEditor* colorFigureEditor = new ColorFigureEditor(lightFigureEditor->x(), lightFigureEditor->y(), lightFigureEditor->infoWidth, lightFigureEditor->infoHeight, lightFigureEditor->label());
    Fl_Choice* typechoice1 = (Fl_Choice*) lightFigureEditor->otherWidgets[0];
    Fl_Choice* typechoice2 = (Fl_Choice*) colorFigureEditor->otherWidgets[0];
    typechoice2->value(typechoice1->value());
    typechoice2->do_callback();
    LightingGroup* lightingGroup = (LightingGroup*) lightFigureEditor->otherWidgets[1];
    Fl_Color_Chooser* chooser = (Fl_Color_Chooser*) colorFigureEditor->otherWidgets[1];
    chooser->rgb(lightingGroup->r(), lightingGroup->g(), lightingGroup->b());
    for(int i = 0; i < lightFigureEditor->inputs.size();i++){
        Fl_Input* input1 = lightFigureEditor->inputs[i];
        Fl_Input* input2 = colorFigureEditor->inputs[i];
        input2->value(input1->value());
    }
    return colorFigureEditor;
}

std::string dtos(double d) {
    std::string s = std::to_string(d);
    long decPoint = s.find('.');
    long firstZero = s.find('0',decPoint);

    return (firstZero == decPoint+1 ? s.substr(0,decPoint) : s.substr(0,firstZero));
}

void imagePreviewTimeOut(void *v) {
    Fl_Double_Window* window = (Fl_Double_Window*) v;
    std::string filename = (std::string)window->label() + ".bmp";

    if (fileExists(filename)){
        Fl_BMP_Image* image = new Fl_BMP_Image(filename.c_str());
        window->size(image->w(), image->h());
        Fl_Box* box = new Fl_Box(0,0,image->w(), image->h());
        box->image(image);
        window->add(box);
        window->redraw();
        window->take_focus();
    }
    else{
        std::cerr << "." << std::endl;
        Fl::repeat_timeout(0.5, imagePreviewTimeOut, v);
    }
}

bool Editor::mouseOverInfo(int mouseX, int mouseY) {
    return mouseX >= displayBox->x() && mouseX < displayBox->x()+infoWidth && mouseY >= displayBox->y() && mouseY < displayBox->y()+infoHeight;
}

bool Editor::mouseOverEdit(int mouseX, int mouseY) {
    return mouseX >= scrollInfo->x() && mouseX < scrollInfo->x()+editWidth && mouseY >= scrollInfo->y() && mouseY < scrollInfo->y()+editHeight;
}

void Editor::rename() {
    const char* text = fl_input("What is the new name?", this->displayBox->label());
    if (text != 0){
        this->label(text);
        displayBox->label(label());
    }
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
    stringR = dtos(r).substr(0,4);
    stringG = dtos(g).substr(0,4);
    stringB = dtos(b).substr(0,4);
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

void LightingGroup::value(double r, double g, double b) {
    Fl_Input* inputR = (Fl_Input*) child(0);
    Fl_Input* inputG = (Fl_Input*) child(1);
    Fl_Input* inputB = (Fl_Input*) child(2);
    inputR->value(dtos(r).c_str());
    inputG->value(dtos(g).c_str());
    inputB->value(dtos(b).c_str());
}

double LightingGroup::r() {
    return std::stod(((Fl_Input*)child(0))->value());
}

double LightingGroup::g() {
    return std::stod(((Fl_Input*)child(1))->value());

}

double LightingGroup::b() {
    return std::stod(((Fl_Input*)child(2))->value());
}


bool fileExists(std::string filename) {
    struct stat buf;
    return (stat(filename.c_str(), &buf) == 0);

}

bool directoryExists(std::string dirname) {
    struct stat buf;
    return (stat(dirname.c_str(), &buf) == 0 && S_ISDIR(buf.st_mode));
}