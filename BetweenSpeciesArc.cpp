#include "BetweenSpeciesArc.h"
#include "SingleSpeciesLineChart.h"
#include <string>
#include "PrintText.h"
#include "Color.h"

BetweenSpeciesArc::BetweenSpeciesArc(float coefficient, SingleSpeciesLineChart *speciesA, SingleSpeciesLineChart *speciesB, std::string label) 
: VerticalArc(0, 0, 10) {
    this->speciesA = speciesA;
    this->speciesB = speciesB;
    this->coefficient = coefficient;
    this->betweenSpeciesLabel = label;
    this->fontHeight = 12;
    fadingAlpha = true;
}

BetweenSpeciesArc::~BetweenSpeciesArc() {

}

void BetweenSpeciesArc::drawFaded() {
    float oldStart = startAlpha;
    float oldFinal = finalAlpha;
    startAlpha = 0.3;
    finalAlpha = 0.05;
    draw();
    startAlpha = oldStart;
    finalAlpha = oldFinal;
}

void BetweenSpeciesArc::draw() {
    if (arcToRight()) {
        this->x = speciesA->getXLocation() + speciesA->getWidth();
    } else {
        this->x = speciesA->getXLocation();
    }

    this->yA = speciesA->getYLocation() + speciesA->getOffsetY() + speciesA->getActualHeight() / 2;
    this->yB = speciesB->getYLocation() + speciesB->getOffsetY() + speciesB->getActualHeight() / 2;

    this->setThickness(getThickness());

    VerticalArc::draw();
}

void BetweenSpeciesArc::drawSelected() {
    if (selected) {
        Color white = Color::white;
        float highlightThickness = 4;
        float radiusOffset = thickness / 2 + highlightThickness / 2;
        float innerRadius = radius - radiusOffset;
        float outerRadius = radius + radiusOffset;
        this->drawLineArc(x, y, innerRadius, highlightThickness, startAngle, arcAngle, &white, 0.75, 0.75);
        this->drawLineArc(x, y, outerRadius, highlightThickness, startAngle, arcAngle, &white, 0.75, 0.75);
        CenteredArc::drawSelected();
      
        std::string label = speciesA->getTitle() + betweenSpeciesLabel + speciesB->getTitle() + " (" + toStr(coefficient) + ")";

        float padding = 4;
        float xPos = this->x + radius / 2;

        if (!this->arcToRight()) {
            xPos = this->x - radius / 2;
        }

        float yPos = this->y;

        glColor4f(color->r, color->g, color->b, 1);
        PrintText::drawStrokeText(label, xPos, yPos, fontHeight, HORIZ_CENTER, VERT_CENTER, true);
    }
}