#include "GraphMarker.h"
#include "Shape.h"
#include "Circle.h"
#include "Square.h"
#include "Triangle.h"
#include "Color.h"
#include "PrintText.h"

GraphMarker::GraphMarker(float vX, float vY, int shape) {
    valueX = vX;
    valueY = vY;
    label = toStr(vY);

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

    setSize(8);
    setBorderWidth(0.0);
    setDisplayLabel(false);
}

GraphMarker::~GraphMarker() {
    delete marker;
}

void GraphMarker::calculateLocation(float graphWidth, float graphHeight,
                                    float maxValueX,  float maxValueY) {
    float posX = valueX * graphWidth /  maxValueX;
    float posY = valueY * graphHeight / maxValueY;

    marker->setLocation(posX, posY);
}

void GraphMarker::draw() {
    marker->draw();

    if (displayLabel) {
        float posX = marker->getX() + 3 *(marker->getWidth() / 4);
        float posY = marker->getY() + (marker->getHeight() / 2);

        glColor4f(0.0, 0.0, 0.0, 1.0);
        PrintText::printAt(posX, posY, label, false, false, GLUT_BITMAP_HELVETICA_12);
    }
}

void GraphMarker::drawToPick() {
    marker->drawToPick();
}

void GraphMarker::setShape(int shape) {
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

void GraphMarker::setSize(float size) {
    marker->setSize(size, size);
}

void GraphMarker::setBorderColor(Color *color) {
    marker->setBorderColor(color);
}

void GraphMarker::setFillColor(Color *color) {
    marker->setFillColor(color);
}

void GraphMarker::setBorderWidth(float width) {
    marker->setBorderWidth(width);
}

float GraphMarker::getPositionX() {
    return marker->getX();
}

float GraphMarker::getPositionY() {
    return marker->getY();
}

void GraphMarker::setPickColor(int r, int g, int b) {
    marker->setPickColor(r, g, b);
}