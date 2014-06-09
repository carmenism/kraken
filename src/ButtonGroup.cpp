#include "ButtonGroup.h"
#include "Button.h"
#include "PrintText.h"
#include <GL/glut.h>

ButtonGroup::ButtonGroup(std::string groupLabels, std::vector<std::string> buttonLabels, int inactiveIndex) {
    this->label = groupLabels;

    size = buttonLabels.size();

    buttons = new Button *[size];

    for (unsigned int i = 0; i < size; i++) {
        buttons[i] = new Button(buttonLabels[i]);
        buttons[i]->setWidth(PrintText::strokeWidth(buttonLabels[i], 10) + 8);
        buttons[i]->setHeight(18);
        
        if (i == inactiveIndex) {
            buttons[i]->setActive(false);
        } else {
            buttons[i]->setActive(true);
        }
    }

    x = 0;
    y = 0;
    width = 0;
    height = 0;
    spacing = 4;
}

ButtonGroup::~ButtonGroup() {
    for (int i = 0; i < size; i++) {
        delete buttons[i];
    }

    delete[] buttons;
}

void ButtonGroup::draw() {
    float textWidth = int(PrintText::strokeWidth(label, 10)) + 1;
    
    float runningWidth = 2 * spacing + textWidth;
    float largestHeight = -1;
    
    for (int i = 0; i < size; i++) {
        buttons[i]->setLocation(x + runningWidth, y + spacing);

        runningWidth = runningWidth + spacing + buttons[i]->getWidth();
        largestHeight = std::max(largestHeight, buttons[i]->getHeight());
    }

    for (int i = 0; i < size; i++) {
        buttons[i]->draw();
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

    for (unsigned int i = 0; i < size; i++) {
        if (buttons[i]->mouseMoved(mouseX, mouseY)) {
            moved = buttons[i];
        }
    }

    return (moved != NULL);
}

bool ButtonGroup::mousePressed(float mouseX, float mouseY) {
    for (unsigned int i = 0; i < size; i++) {
        if (buttons[i]->mousePressed(mouseX, mouseY)) {
            return true;
        }
    }

    return false;
}

bool ButtonGroup::mouseReleased(float mouseX, float mouseY) {
    releasedIndex = -1;

    for (unsigned int i = 0; i < size; i++) {
        if (buttons[i]->mouseReleased(mouseX, mouseY)) {
            releasedIndex = i;
        }
    }

    if (releasedIndex != -1) {
        for (unsigned int i = 0; i < size; i++) {
            if (i == releasedIndex) {
                buttons[i]->activeOff();
            } else {
                buttons[i]->activeOn();
            }
        }

        return true;
    }

    return false;
}

bool ButtonGroup::getActive(int buttonIndex) {
    if (buttonIndex >= 0 && buttonIndex < size) {
        return buttons[buttonIndex]->isActive();
    }

    return false;
}

void ButtonGroup::setActive(int buttonIndex, bool a) {
    if (buttonIndex >= 0 && buttonIndex < size) {
        buttons[buttonIndex]->setActive(a);
    }
}

void ButtonGroup::activeOff() {
    for (unsigned int i = 0; i < size; i++) {
        buttons[i]->activeOff();
    }
}