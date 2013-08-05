#include "InteractionArc.h"
#include "SingleSpeciesLineChart.h"
#include <string>
#include "PrintText.h"
#include "Color.h"

InteractionArc::InteractionArc(float coefficient, SingleSpeciesLineChart *speciesA, SingleSpeciesLineChart *speciesB) 
: VerticalArc(0, 0, 10) {
    this->speciesA = speciesA;
    this->speciesB = speciesB;
    this->coefficient = coefficient;
    //this->displayLabel = false;
    this->fontHeight = 10;
    fadingAlpha = true;
}

InteractionArc::~InteractionArc() {

}

void InteractionArc::draw() {
    if (arcToRight()) {
        this->x = speciesA->getXLocation() + speciesA->getWidth();
    } else {
        this->x = speciesA->getXLocation();
    }

    this->yA = speciesA->getYLocation() + speciesA->getHeight() / 2;
    this->yB = speciesB->getYLocation() + speciesB->getHeight() / 2;
    this->setThickness(coefficient * 30);

    VerticalArc::draw();
}

void InteractionArc::drawSelected() {
    if (selected) {
        CenteredArc::drawSelected();

        std::string label = speciesA->getTitle() + " interacts with " + speciesB->getTitle() + " (" + toStr(coefficient) + ")";

        float padding = 4;
        float xPos = this->x + radius / 2;//mouseX + padding;
        float yPos = this->y;//mouseY + padding;

        float h = fontHeight;
        float w = PrintText::strokeWidth(label, fontHeight);

        glPolygonMode(GL_FRONT, GL_FILL);  
        glColor4f(1.0, 1.0, 1.0, 0.85);

        glRectf(xPos - padding,     yPos - padding,
                xPos + w + padding, yPos + h + padding);
        glColor4f(color->r, color->g, color->b, 1);
        PrintText::drawStrokeText(label, xPos, yPos, fontHeight);
    }
}