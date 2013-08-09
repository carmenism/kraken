#include "ChartPoint.h"
#include "LineChart.h"
#include "Shape.h"
#include "Circle.h"
#include "Square.h"
#include "Triangle.h"
#include "Color.h"
#include "PrintText.h"
#include <iostream>

ChartPoint::ChartPoint(LineChart *chart, std::string label, float valueX, float valueY, int shape)
 : Point(valueX, valueY), Pickable() {
    last = new Point(valueX, valueY);
    this->chart = chart;
    this->label = label;

    switch (shape) {
        case SHAPE_SQUARE:
            marker = new Square();
            break;
        case SHAPE_TRIANGLE:
            marker = new Triangle();
            break;
        case SHAPE_CIRCLE:
        default:
            marker = new Circle();
            break;
    }

    marker->setFillColor(new Color(0, 0, 0, 1));
    marker->setBorderColor(new Color(0, 0, 0, 1));

    setSize(8);
    setBorderWidth(0.0);
    //setDisplayLabel(false);
    setFontHeight(14);
}

ChartPoint::~ChartPoint() {
    delete marker;
}

void ChartPoint::calculateLocation() {
    float posX = x * chart->getActualWidth() / chart->getGlobalMaxX();
    float posY = y * chart->getActualHeight() / chart->getGlobalMaxY();

    marker->setLocation(posX, posY);

    lastPositionX = last->getX() * chart->getActualWidth() / chart->getGlobalMaxX();
    lastPositionY = last->getY() * chart->getActualHeight() / chart->getGlobalMaxY();
}

void ChartPoint::draw() {
    marker->draw();
}

void ChartPoint::drawSelected() {
    if (selected) {
        float oldWidth = marker->getWidth();
        float oldHeight = marker->getHeight();
        marker->setSize(oldWidth * 1.5, oldHeight * 1.5);
        marker->draw();
        marker->setSize(oldWidth, oldHeight);

        Color *c = marker->getFillColor();

        drawHistoryLine();
        drawLineToXAxis();

        std::string newLabel = makeLabel();

        float padding = 4;

        float x = marker->getX() + 3 *(marker->getWidth() / 4) + padding;
        float y = marker->getY() + (marker->getHeight() / 2) + padding;

        float h = fontHeight;
        float w = PrintText::strokeWidth(newLabel, fontHeight);

        if (x + w > chart->getActualWidth()) {
            x = x - w - 4 * padding;         
        }
        if (x < 0) {
            x = 0;
        }

        if (y + h > chart->getActualHeight()) {
            y = y - h - 4 * padding;
        }

        glPolygonMode(GL_FRONT, GL_FILL);  
        glColor4f(1.0, 1.0, 1.0, 0.85);

        glRectf(x - padding,     y - padding,
                x + w + padding, y + h + padding);

        //glColor4f(0.0, 0.0, 0.0, 1.0);
        glColor4f(c->r, c->g, c->b, 1);
        PrintText::drawStrokeText(newLabel, x, y, fontHeight);
    }
}

std::string ChartPoint::makeLabel() {
    std::string newLabel = label + ": " + toStr(y);

    float diff = y - last->getY();

    if (diff > 0) {
        newLabel = newLabel + " (+" + toStr(diff) + ")";
    } else if (diff < 0) {
        newLabel = newLabel + " (" + toStr(diff) + ")";
    }

    return newLabel;
}

void ChartPoint::drawHistoryLine() {
    Color *c = marker->getBorderColor();

    glEnable(GL_SCISSOR_TEST);

    float startX = chart->getOffsetX() + chart->getXLocation();
    float startY = chart->getOffsetY() + chart->getYLocation();

    glScissor(startX - 2, startY, chart->getActualWidth() + 4, chart->getActualHeight());

    glColor4f(c->r, c->g, c->b, 0.6);
    glLineWidth(3);
    glBegin(GL_LINES);
        glVertex3f(marker->getX(), marker->getY(), 0);
        glVertex3f(lastPositionX, lastPositionY, 0);
    glEnd();
    glLineWidth(1);

    glDisable(GL_SCISSOR_TEST);
}

void ChartPoint::drawLineToXAxis() {
    float diff = y - last->getY();

    glColor4f(0, 0, 0, 0.3);    

    if (diff > 0) {
        glBegin(GL_LINES);
            glVertex3f(lastPositionX, lastPositionY, 0);
            glVertex3f(marker->getX(), 0, 0);
        glEnd();
    } else {
        float f = marker->getHeight() / 2;

        glBegin(GL_LINES);
            glVertex3f(marker->getX(), marker->getY() - f, 0);
            glVertex3f(lastPositionX, 0, 0);
        glEnd();
    }
}

void ChartPoint::drawToPick() {
    marker->setPickColor(pickR, pickG, pickB);
    float oldWidth = marker->getWidth();
    float oldHeight = marker->getHeight();
    marker->setSize(oldWidth * 1.5, oldHeight * 1.5);
    marker->drawToPick();
    marker->setSize(oldWidth, oldHeight);
}

void ChartPoint::setShape(int shape) {
    Shape *newMarker;

    switch (shape) {
        case SHAPE_SQUARE:
            newMarker = new Square();
            break;
        case SHAPE_TRIANGLE:
            newMarker = new Triangle();
            break;
        case SHAPE_CIRCLE:
        default:
            newMarker = new Circle();
            break;
    }

    newMarker->setSize(marker->getWidth(), marker->getHeight());
    newMarker->setLocation(marker->getX(), marker->getY());
    newMarker->setRotation(marker->getRotation());
    newMarker->setDrawFill(marker->getDrawFill());
    newMarker->setDrawBorder(marker->getDrawBorder());
    newMarker->setFillColor(new Color(*marker->getFillColor()));
    newMarker->setBorderColor(new Color(*marker->getBorderColor()));

    delete marker;

    marker = newMarker;
}

void ChartPoint::setSize(float size) {
    marker->setSize(size, size);
}

void ChartPoint::setBorderColor(Color *color) {
    marker->setBorderColor(color);
}

void ChartPoint::setFillColor(Color *color) {
    marker->setFillColor(color);
}

void ChartPoint::setBorderWidth(float width) {
    marker->setBorderWidth(width);
}

float ChartPoint::getPositionX() {
    return marker->getX();
}

float ChartPoint::getPositionY() {
    return marker->getY();
}
/*
void ChartPoint::setPickColor(int r, int g, int b) {
    marker->setPickColor(r, g, b);
}*/

void ChartPoint::setPositionX(float x) {
    marker->setLocation(x, marker->getY());
}

void ChartPoint::setPositionY(float y) {
    marker->setLocation(marker->getX(), y);
}

void ChartPoint::captureLastValues() {
    last->setValues(x, y);
    lastPositionX = marker->getX();
    lastPositionY = marker->getY();
}