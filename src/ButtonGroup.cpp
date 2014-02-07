#include "ButtonGroup.h"
#include "Button.h"
#include "PrintText.h"
#include <GL/glut.h>

ButtonGroup::ButtonGroup(std::string groupLabels, std::vector<std::string> buttonLabels, int inactiveIndex) {
    this->label = groupLabels;

    buttons = new std::vector<Button *>();

    for (unsigned int i = 0; i < buttonLabels.size(); i++) {
        Button *b = new Button(buttonLabels[i]);
        b->setWidth(PrintText::strokeWidth(buttonLabels[i], 10) + 8);
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
        largestHeight = std::max(largestHeight, buttons->at(i)->getHeight());
    }

    for (int i = 0; i < buttons->size(); i++) {
        buttons->at(i)->draw();
    }

    this->width = runningWidth;
    this->height = largestHeight + 2 * spacing;

    glColor4f(0, 0, 0, 1);
    PrintText::drawStrokeText(label, x + spacing, y + height / 2, 10, HORIZ_LEFT, VERT_CENTER);
    
    glPushMatrix();
        glTranslatef(x, y, 0);
        drawBoundary();
    glPopMatrix();
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

bool ButtonGroup::getActive(int buttonIndex) {
    if (buttonIndex >= 0 && buttonIndex < buttons->size()) {
        return buttons->at(buttonIndex)->isActive();
    }

    return false;
}

void ButtonGroup::setActive(int buttonIndex, bool a) {
    if (buttonIndex >= 0 && buttonIndex < buttons->size()) {
        buttons->at(buttonIndex)->setActive(a);
    }
}

void ButtonGroup::activeOff() {
    for (unsigned int i = 0; i < buttons->size(); i++) {
        buttons->at(i)->activeOff();
    }
}