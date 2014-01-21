#include "InterSpeciesArc.h"
#include "SmallMultiple.h"
#include <string>

#include "Triangle.h"
#include <QtOpenGL>
#include "PrintText.h"
#include "Color.h"
#include "PlotManager.h"

InterSpeciesArc::InterSpeciesArc(PlotManager *pm, float coefficient, SmallMultiple *source, SmallMultiple *recipient, std::string label) 
: CenteredArc() {
    this->yA = yA;
    this->yB = yB;

    setArcToRight();

    this->plotManager = pm;
    this->source = source;
    this->recipient = recipient;
    this->coefficient = coefficient;
    this->betweenSpeciesLabel = label;
    this->fontHeight = 12;


    adjustType = ADJUST_NONE;
    adjustPercentage = 0.4;
}

InterSpeciesArc::~InterSpeciesArc() {

}

void InterSpeciesArc::drawFaded() {
    float oldStart = startAlpha;
    float oldFinal = finalAlpha;
    float oldSignAlpha = signAlpha;
    startAlpha = 0.3;
    finalAlpha = 0.05;
    signAlpha = 0.3;

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
    signAlpha = oldSignAlpha;

    arrowA->getBorderColor()->a = oldArrowBord;
    arrowA->getFillColor()->a = oldArrowFill;
    arrowB->getBorderColor()->a = oldArrowBord;
    arrowB->getFillColor()->a = oldArrowFill;
    arrowMiddle->getBorderColor()->a = oldArrowBord;
    arrowMiddle->getFillColor()->a = oldArrowFill;
}

void InterSpeciesArc::draw() {
    setUpForDrawing();

    if (thickness != 0) {
        CenteredArc::draw();    

        //if (!displayDynamically 
        //    || (displayDynamically && !animated)) {
            
        //}

        if (displayDynamically && !selected) {
            drawSigns();
        }
    }
}

void InterSpeciesArc::drawToPick() {
    setUpForDrawing();

    if (thickness != 0) {
        CenteredArc::drawToPick();
    }
}

void InterSpeciesArc::setUpForDrawing() {
    if (isArcToRight()) {
        this->x = source->getXLocation() + source->getWidth();
    } else {
        this->x = source->getXLocation();// + source->getOffsetX();
    }

    this->yA = source->getYLocation() + source->getOffsetY() + source->getInnerHeight() / 2;
    this->yB = recipient->getYLocation() + recipient->getOffsetY() + recipient->getInnerHeight() / 2;
    
    adjustPositions();             
    determineCenterLocation();
    determineThickness(); 
    positionTriangles();
}

void InterSpeciesArc::determineThickness() {
    if (displayDynamically) {     
        this->setThickness(getDynamicThickness());        
    } else {        
        this->setThickness(min(30, 30 * getCoefficent()));
    }
}

void InterSpeciesArc::adjustPositions() {
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
}

float InterSpeciesArc::getDynamicThickness() {
    float coef = getCoefficient();

    float increaseSource = source->getPercentIncreaseOfFinalValue();
    //float increaseRecipient = speciesB->getPercentIncreaseOfFinalValue();
    
    float finalSource = source->getFinalValue();
    float prevSource = source->getPreviousFinalValue();
    float startRecipient = recipient->getStartValue();
    float finalRecipient = recipient->getFinalValue();

    float recipWeight = 100000.0 / (startRecipient + 100000.0);

    float mult = getDynamicConstant();
    float thick = mult * coef * increaseSource * prevSource * recipWeight * 0.3;
    
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

void InterSpeciesArc::drawSelected() {
    if (selected) {
        setUpForDrawing();

        CenteredArc::drawSelected();
        
        if (displayDynamically) {
            drawSigns();
        }
      
        std::string label = source->getTitle() + betweenSpeciesLabel + recipient->getTitle() + " (" + toStr(coefficient) + ")";

        float xPos = this->x + radius / 2;

        if (!this->isArcToRight()) {
            xPos = this->x - radius / 2;
        }

        float yPos = this->y;

        glLineWidth(1.0);
        glColor4f(0, 0, 0, 1);
        PrintText::drawStrokeText(label, xPos, yPos, fontHeight, HORIZ_CENTER, VERT_CENTER, true);
    }
}

void InterSpeciesArc::determineCenterLocation() {
    radius = fabs(yA - yB) / 2;
    y = min(yA, yB) + radius;
}