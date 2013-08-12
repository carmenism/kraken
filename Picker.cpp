#include "Picker.h"
#include "Pickable.h"
#include "MyQGLWidget.h"
#include <QtOpenGL>

Picker::Picker(MyQGLWidget *widget) {
    this->widget = widget;
    hovered = NULL;
}

void Picker::pick(std::vector<Pickable *> *allPickables, int x, int y) {
    float color[4];    
    unsigned char val[3] = {'\0'};
    unsigned int pick;

    glGetFloatv(GL_COLOR_CLEAR_VALUE, color);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(color[0], color[1], color[2], color[3]);

    for (unsigned int i = 0; i < allPickables->size(); i++) {
        allPickables->at(i)->setPickColor(i & 0xFF, (i >> 8) & 0xFF, 0);
    }

    glDisable(GL_BLEND);
    widget->drawToPick();
    glEnable(GL_BLEND);

    glFlush();
    glReadBuffer(GL_BACK);
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, val);
    
    pick = (val[1] << 8) + val[0];

    int size = allPickables->size();
    if (pick >=0 && pick < size) {
        setHovered(allPickables->at(pick), x, y);
    }
    else if (hovered != NULL) {
        hovered->selectedOff();
        hovered = NULL;
    }
}

void Picker::setHovered(Pickable *pickable, float x, float y) {
    pickable->selectedOn();
    
    if (hovered != pickable) {
        if (hovered != NULL) {
            hovered->selectedOff();
            hovered = NULL;
        }
        
        hovered = pickable;
    }

    hovered->setMouseX(x);
    hovered->setMouseY(y);
}

//void Picker::drawPicked() {
//    hovered->drawSelected();
//}