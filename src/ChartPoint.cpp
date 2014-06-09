#include "ChartPoint.h"
#include "LineChart.h"
#include "Shape.h"
#include "Color.h"
#include "PrintText.h"
#include <iostream>

ChartPoint::ChartPoint(LineChart *chart, std::string label, float valueX, float valueY)
 : Point(valueX, valueY), Pickable() {
    position = new Point(0, 0);
    previousPosition = new Point(0, 0);
    previousValues = new Point(valueX, valueY);
    this->chart = chart;
    this->label = label;

    //setDisplayLabel(false);
    setFontHeight(14);
}

ChartPoint::~ChartPoint() {
    delete previousValues;
    delete position;
    delete previousPosition;
}

void ChartPoint::calculateLocation() {
    float posX = x * chart->getInnerWidth() / chart->getGlobalMaxX();
    float posY = y * chart->getInnerHeight() / chart->getGlobalMaxY();

    position->setValues(posX, posY);

    previousPosition->setX(previousValues->getX() * chart->getInnerWidth() / chart->getGlobalMaxX());
    previousPosition->setY(previousValues->getY() * chart->getInnerHeight() / chart->getGlobalMaxY());
}

void ChartPoint::draw() {
    marker->setX(position->getX());
    marker->setY(position->getY());

    marker->draw();
}

void ChartPoint::drawSelected() {    
    marker->setX(position->getX());
    marker->setY(position->getY());

    if (selected) {
        float oldWidth = marker->getWidth();
        float oldHeight = marker->getHeight();
        marker->setX(position->getX());
        marker->setY(position->getY());
        marker->setSize(oldWidth * 1.5, oldHeight * 1.5);
        marker->draw();
        marker->setSize(oldWidth, oldHeight);

        drawHistoryLine();
        drawLineToXAxis();

        std::string newLabel = makeLabel();

        float padding = 4;

        float x = position->getX() + 3 *(marker->getWidth() / 4) + padding;
        float y = position->getY() + (marker->getHeight() / 2) + padding;

        float h = fontHeight;
        float w = PrintText::strokeWidth(newLabel, fontHeight);

        if (x + w > chart->getInnerWidth()) {
            x = x - w - 4 * padding;         
        }
        if (x < 0) {
            x = 0;
        }

        if (y + h > chart->getInnerHeight()) {
            y = y - h - 4 * padding;
        }

        glColor4f(0, 0, 0, 1);
        PrintText::drawStrokeText(newLabel, x, y, fontHeight, HORIZ_LEFT, VERT_BOTTOM, true);
    }
}

std::string ChartPoint::makeLabel() {
    std::string newLabel = label + ": " + toStr(y);

    float diff = y - previousValues->getY();

    if (diff > 0) {
        newLabel = newLabel + " (+" + toStr(diff) + ")";
    } else if (diff < 0) {
        newLabel = newLabel + " (" + toStr(diff) + ")";
    }

    return newLabel;
}

void ChartPoint::drawHistoryLine() {
    marker->setX(position->getX());
    marker->setY(position->getY());

    Color *c = marker->getBorderColor();

    glEnable(GL_SCISSOR_TEST);

    float startX = chart->getOffsetX() + chart->getX();
    float startY = chart->getOffsetY() + chart->getY();

    glScissor(startX - 2, startY, chart->getInnerWidth() + 4, chart->getInnerHeight());

    glEnable(GL_LINE_SMOOTH);
    glColor4f(c->r, c->g, c->b, 0.6);
    glLineWidth(3);
    glBegin(GL_LINES);
        glVertex3f(marker->getX(), marker->getY(), 0);
        glVertex3f(previousPosition->getX(), previousPosition->getY(), 0);
    glEnd();
    glLineWidth(1);

    glDisable(GL_SCISSOR_TEST);
}

void ChartPoint::drawLineToXAxis() {    
    marker->setX(position->getX());
    marker->setY(position->getY());

    float diff = y - previousValues->getY();
    
    glDisable(GL_LINE_SMOOTH);
    glColor4f(0, 0, 0, 0.3);    

    if (diff > 0) {
        glBegin(GL_LINES);
            glVertex3f(previousPosition->getX(), previousPosition->getY(), 0);
            glVertex3f(marker->getX(), 0, 0);
        glEnd();
    } else {
        float f = marker->getHeight() / 2;

        glBegin(GL_LINES);
            glVertex3f(marker->getX(), marker->getY() - f, 0);
            glVertex3f(previousPosition->getX(), 0, 0);
        glEnd();
    }

    glEnable(GL_LINE_SMOOTH);
}

void ChartPoint::drawToPick() {
    marker->setX(position->getX());
    marker->setY(position->getY());

    marker->setPickColor(pickR, pickG, pickB);
    float oldWidth = marker->getWidth();
    float oldHeight = marker->getHeight();
    marker->setSize(oldWidth * 1.5, oldHeight * 1.5);
    marker->drawToPick();
    marker->setSize(oldWidth, oldHeight);
}

float ChartPoint::getPositionX() {
    return position->getX();
}

float ChartPoint::getPositionY() {
    return position->getY();
}

void ChartPoint::setPositionX(float x) {
    position->setX(x);
}

void ChartPoint::setPositionY(float y) {
    position->setY(y);
}

void ChartPoint::capturePreviousValues() {
    previousValues->setValues(x, y);
    previousPosition->setX(position->getX());
    previousPosition->setY(position->getY());
}

float ChartPoint::getDifferenceFromPrevious() {
    return y - previousValues->getY();
}

float ChartPoint::getPercentIncreaseFromPrevious() {
    return getDifferenceFromPrevious() / previousValues->getY();
}

float ChartPoint::getPreviousValue() {
    return previousValues->getY();
}