#include "ButtonGroup.h"
#include "Button.h"
#include "PrintText.h"
#include <QtOpenGL>

ButtonGroup::ButtonGroup(std::string groupLabels, std::vector<std::string> buttonLabels, int inactiveIndex) {
    this->label = groupLabels;

    buttons = new std::vector<Button *>();

    for (unsigned int i = 0; i < buttonLabels.size(); i++) {
        Button *b = new Button(buttonLabels[i]);
        b->setWidth(55);
        b->setHeight(18);
        
        if (i == inactiveIndex) {
            b->setActive(false);
        } else {
            b->setActive(true);
        }

        buttons->push_back(b);
    }

    x = 0;
    y = 0;
    width = 0;
    height = 0;
    spacing = 4;
}

ButtonGroup::~ButtonGroup() {
    while (!buttons->empty()) {
        Button *b = buttons->back();
        buttons->pop_back();

        delete b;
    }

    delete buttons;
}

void ButtonGroup::draw() {
    float textWidth = int(PrintText::strokeWidth(label, 10)) + 1;
    
    float runningWidth = 2 * spacing + textWidth;
    float largestHeight = -1;
    
    for (int i = 0; i < buttons->size(); i++) {
        buttons->at(i)->setLocation(x + runningWidth, y + spacing);

        runningWidth = runningWidth + spacing + buttons->at(i)->getWidth();
        largestHeight = max(largestHeight, buttons->at(i)->getHeight());
    }

    for (int i = 0; i < buttons->size(); i++) {
        buttons->at(i)->draw();
    }

    this->width = runningWidth;
    this->height = largestHeight + 2 * spacing;

    glColor4f(0, 0, 0, 1);
    PrintText::drawStrokeText(label, x + spacing, y + height / 2, 10, HORIZ_LEFT, VERT_CENTER);
    
    drawBox();
}

void ButtonGroup::drawBox() {
    glDisable(GL_LINE_SMOOTH);
    glPolygonMode(GL_FRONT, GL_LINE);  
        glLineWidth(1);
        glColor4f(0.5, 0.5, 0.5, 1);

        glBegin(GL_LINE_LOOP);
        glVertex2f( x, y );
        glVertex2f( x, y + height );
        glVertex2f( x + width, y + height );
        glVertex2f( x + width, y );
    glEnd();
    glEnable(GL_LINE_SMOOTH);
}

bool ButtonGroup::mouseMoved(float mouseX, float mouseY) {
    Button *moved = NULL;

    for (unsigned int i = 0; i < buttons->size(); i++) {
        if (buttons->at(i)->mouseMoved(mouseX, mouseY)) {
            moved = buttons->at(i);
        }
    }

    return (moved != NULL);
}

bool ButtonGroup::mousePressed(float mouseX, float mouseY) {
    for (unsigned int i = 0; i < buttons->size(); i++) {
        if (buttons->at(i)->mousePressed(mouseX, mouseY)) {
            return true;
        }
    }

    return false;
}

bool ButtonGroup::mouseReleased(float mouseX, float mouseY) {
    releasedIndex = -1;

    for (unsigned int i = 0; i < buttons->size(); i++) {
        if (buttons->at(i)->mouseReleased(mouseX, mouseY)) {
            releasedIndex = i;
        }
    }

    if (releasedIndex != -1) {
        for (unsigned int i = 0; i < buttons->size(); i++) {
            if (i == releasedIndex) {
                buttons->at(i)->activeOff();
            } else {
                buttons->at(i)->activeOn();
            }
        }

        return true;
    }

    return false;
}