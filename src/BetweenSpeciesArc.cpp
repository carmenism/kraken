#include "BetweenSpeciesArc.h"
#include "SmallMultiple.h"
#include <string>

#include "PrintText.h"
#include "Color.h"
#include "Triangle.h"
#include "PlotManager.h"

BetweenSpeciesArc::BetweenSpeciesArc(PlotManager *pm, float coefficient, SmallMultiple *source, SmallMultiple *recipient, std::string label) 
: VerticalArc(0, 0, 10) {
    this->plotManager = pm;
    this->source = source;
    this->recipient = recipient;
    this->coefficient = coefficient;
    this->betweenSpeciesLabel = label;
    this->fontHeight = 12;
    fadingAlpha = true;

    arrowA = new Triangle();
    arrowA->setBorderColor(new Color(0, 0, 0, .95));
    arrowA->setFillColor(new Color(1, 1, 1, .65));
    arrowA->setSize(10, 10);

    arrowB = new Triangle();
    arrowB->setBorderColor(new Color(0, 0, 0, .95));
    arrowB->setFillColor(new Color(1, 1, 1, .65));
    arrowB->setSize(10, 10);
    
    arrowMiddle = new Triangle();
    arrowMiddle->setBorderColor(new Color(0, 0, 0, .95));
    arrowMiddle->setFillColor(new Color(1, 1, 1, .65));
    arrowMiddle->setSize(10, 10);

    displayDynamically = false;

    adjustType = ADJUST_NONE;
    adjustPercentage = 0.4;
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
    arrowA->getBorderColor()->a = startAlpha;
    arrowA->getFillColor()->a = finalAlpha;
    arrowB->getBorderColor()->a = startAlpha;
    arrowB->getFillColor()->a = finalAlpha;
    arrowMiddle->getBorderColor()->a = startAlpha;
    arrowMiddle->getFillColor()->a = finalAlpha;

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
        this->x = source->getXLocation() + source->getWidth();
    } else {
        this->x = source->getXLocation() + source->getOffsetX();
    }

    this->yA = source->getYLocation() + source->getOffsetY() + source->getInnerHeight() / 2;
    this->yB = recipient->getYLocation() + recipient->getOffsetY() + recipient->getInnerHeight() / 2;
    
    float adjustOffset = (source->getInnerHeight() / 2) * adjustPercentage;

    if (adjustType == ADJUST_LARGER) {
        if (this->yA > this->yB) {
            this->yA = this->yA + adjustOffset;
            this->yB = this->yB - adjustOffset;
        } else {
            this->yA = this->yA - adjustOffset;
            this->yB = this->yB + adjustOffset;
        }
    } else if (adjustType == ADJUST_SMALLER) {
        if (this->yA > this->yB) {
            this->yA = this->yA - adjustOffset;
            this->yB = this->yB + adjustOffset;
        } else {
            this->yA = this->yA + adjustOffset;
            this->yB = this->yB - adjustOffset;
        }
    }

    if (displayDynamically) {       
        float newThickness = getDynamicThickness();
        this->setThickness(newThickness);

        if (newThickness != 0) { // && multB > 0.1
            VerticalArc::draw();

            positionTriangles();
            drawTriangles();
        }
    } else {        
        this->setThickness(min(30, 30 * getCoefficent()));

        VerticalArc::draw();

        positionTriangles();
        drawTriangles();
    }
}

float BetweenSpeciesArc::getDynamicThickness() {
    float coef = getCoefficient();

    float increaseSource = source->getPercentIncreaseOfFinalValue();
    //float increaseRecipient = speciesB->getPercentIncreaseOfFinalValue();

    float finalSource = source->getFinalValue();
    float finalRecipient = recipient->getFinalValue();

    //float largest = plotManager->getLargestFinalValue();
    
    //if (increaseA == 0 || increaseB == 0) {
    //    return 0;
    //}

    float thick = 10 * coef * increaseSource * finalSource / finalRecipient;
    
    float maxThickness = 30;

    if (thick > maxThickness) {
        return maxThickness;
    } else if (thick < -maxThickness) {
        return -maxThickness;
    } else if (thick < 1 && thick > -1) {
        return 0;
    }

    return thick;
}

void BetweenSpeciesArc::positionTriangles() {
    Color *c = this->getColor();

    arrowA->setBorderColor(c);
    arrowB->setBorderColor(c);    
    arrowMiddle->setBorderColor(c);

    arrowA->setBorderWidth(2);
    arrowB->setBorderWidth(2);
    arrowMiddle->setBorderWidth(1.5);

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
      
        std::string label = source->getTitle() + betweenSpeciesLabel + recipient->getTitle() + " (" + toStr(coefficient) + ")";

        float xPos = this->x + radius / 2;

        if (!this->arcToRight()) {
            xPos = this->x - radius / 2;
        }

        float yPos = this->y;

        glColor4f(0, 0, 0, 1);
        PrintText::drawStrokeText(label, xPos, yPos, fontHeight, HORIZ_CENTER, VERT_CENTER, true);
    }
}
