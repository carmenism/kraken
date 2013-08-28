#include "BetweenSpeciesArc.h"
#include "SingleSpeciesLineChart.h"
#include <string>

#include "PrintText.h"
#include "Color.h"
#include "Triangle.h"

BetweenSpeciesArc::BetweenSpeciesArc(float coefficient, SingleSpeciesLineChart *speciesA, SingleSpeciesLineChart *speciesB, std::string label) 
: VerticalArc(0, 0, 10) {
    this->speciesA = speciesA;
    this->speciesB = speciesB;
    this->coefficient = coefficient;
    this->betweenSpeciesLabel = label;
    this->fontHeight = 12;
    fadingAlpha = true;

    arrowA = new Triangle();
    arrowA->setBorderColor(new Color(0, 0, 0, .95));
    arrowA->setFillColor(new Color(1, 1, 1, .45));
    arrowA->setSize(10, 10);

    arrowB = new Triangle();
    arrowB->setBorderColor(new Color(0, 0, 0, .95));
    arrowB->setFillColor(new Color(1, 1, 1, .45));
    arrowB->setSize(10, 10);
    
    arrowMiddle = new Triangle();
    arrowMiddle->setBorderColor(new Color(0, 0, 0, .95));
    arrowMiddle->setFillColor(new Color(1, 1, 1, .45));
    arrowMiddle->setSize(10, 10);
}

BetweenSpeciesArc::~BetweenSpeciesArc() {

}

void BetweenSpeciesArc::drawFaded() {
    float oldStart = startAlpha;
    float oldFinal = finalAlpha;
    startAlpha = 0.3;
    finalAlpha = 0.05;

    float oldArrowBord = arrowA->getBorderColor()->a;
    float oldArrowFill = arrowA->getFillColor()->a;
    arrowA->getBorderColor()->a = 0.3;
    arrowA->getFillColor()->a = 0.05;
    arrowB->getBorderColor()->a = 0.3;
    arrowB->getFillColor()->a = 0.05;
    arrowMiddle->getBorderColor()->a = 0.3;
    arrowMiddle->getFillColor()->a = 0.05;

    draw();

    startAlpha = oldStart;
    finalAlpha = oldFinal;

    arrowA->getBorderColor()->a = oldArrowBord;
    arrowA->getFillColor()->a = oldArrowFill;
    arrowB->getBorderColor()->a = oldArrowBord;
    arrowB->getFillColor()->a = oldArrowFill;
    arrowMiddle->getBorderColor()->a = oldArrowBord;
    arrowMiddle->getFillColor()->a = oldArrowFill;
}

void BetweenSpeciesArc::draw() {
    if (arcToRight()) {
        this->x = speciesA->getXLocation() + speciesA->getWidth();
    } else {
        this->x = speciesA->getXLocation() + speciesA->getOffsetX();
    }

    this->yA = speciesA->getYLocation() + speciesA->getOffsetY() + speciesA->getInnerHeight() / 2;
    this->yB = speciesB->getYLocation() + speciesB->getOffsetY() + speciesB->getInnerHeight() / 2;

    this->setThickness(getThickness());

    VerticalArc::draw();

    positionTriangles();
    drawTriangles();
}

void BetweenSpeciesArc::positionTriangles() {
    Color *c = this->getColor();

    arrowA->setBorderColor(c);
    arrowB->setBorderColor(c);    
    arrowMiddle->setBorderColor(c);

    float shortEdge = getRadius() / 2;
    float longEdge = shortEdge * sqrt(3.0f);
    float size = max(4, getThickness() / 2);

    if (arcToRight()) {
        arrowA->setLocation(getX() + longEdge, getY() + shortEdge);
        arrowA->setSize(size, size * 2);
        arrowA->setRotation(210);

        arrowB->setLocation(getX() + longEdge, getY() - shortEdge);
        arrowB->setSize(size, size * 2);
        arrowB->setRotation(150);

        arrowMiddle->setLocation(getX() + getRadius(), getY() - size / 3);
        arrowMiddle->setSize(size, size * 2);
        arrowMiddle->setRotation(180);
    } else {
        arrowA->setLocation(getX() - longEdge, getY() - shortEdge);
        arrowA->setSize(size, size * 2);
        arrowA->setRotation(30);
        
        arrowB->setLocation(getX() - longEdge, getY() + shortEdge);
        arrowB->setSize(size, size * 2);
        arrowB->setRotation(330);
        
        arrowMiddle->setLocation(getX() - getRadius(), getY() + size / 3);
        arrowMiddle->setSize(size, size * 2);
        arrowMiddle->setRotation(0);
    }
}

void BetweenSpeciesArc::drawTriangles() {
    if (selected) {
        arrowA->draw();
        arrowB->draw();
    } else {
        arrowMiddle->draw();
    }
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
        
        drawTriangles();
      
        std::string label = speciesA->getTitle() + betweenSpeciesLabel + speciesB->getTitle() + " (" + toStr(coefficient) + ")";

        float xPos = this->x + radius / 2;

        if (!this->arcToRight()) {
            xPos = this->x - radius / 2;
        }

        float yPos = this->y;

        glColor4f(0, 0, 0, 1);
        PrintText::drawStrokeText(label, xPos, yPos, fontHeight, HORIZ_CENTER, VERT_CENTER, true);
    }
}