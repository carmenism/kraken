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
        Color white = Color::white;
        float highlightThickness = 4;
        float radiusOffset = thickness / 2 + highlightThickness / 2;
        float innerRadius = radius - radiusOffset;
        float outerRadius = radius + radiusOffset;
        this->drawLineArc(x, y, innerRadius, highlightThickness, startAngle, arcAngle, &white, 0.75, 0.75);
        this->drawLineArc(x, y, outerRadius, highlightThickness, startAngle, arcAngle, &white, 0.75, 0.75);
        CenteredArc::drawSelected();
      
        std::string label = speciesA->getTitle() + " interacts with " + speciesB->getTitle() + " (" + toStr(coefficient) + ")";

        float padding = 4;
        float xPos = this->x + radius / 2;

        if (!this->arcToRight()) {
            xPos = this->x - radius / 2;
        }

        float yPos = this->y;

        float h = fontHeight;
        float w = PrintText::strokeWidth(label, fontHeight);

        float xOffset = w / 2 + padding;
        float yOffset = h / 2 + padding;
        
        glPolygonMode(GL_FRONT, GL_FILL);  
        glColor4f(1.0, 1.0, 1.0, 0.85);
        glRectf(xPos - xOffset, yPos - yOffset,
                xPos + xOffset, yPos + yOffset);

        glColor4f(color->r, color->g, color->b, 1);
        PrintText::drawStrokeText(label, xPos, yPos, fontHeight, HORIZ_CENTER, VERT_CENTER);
    }
}