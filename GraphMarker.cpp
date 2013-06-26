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
}

void GraphMarker::drawLabel() {
    void *font = GLUT_BITMAP_HELVETICA_12;

    if (displayLabel) {
        float x = marker->getX() + 3 *(marker->getWidth() / 4);
        float y = marker->getY() + (marker->getHeight() / 2);

        float w = PrintText::printingWidth(label, font);
        float h = PrintText::printingHeight(font);

        glPolygonMode(GL_FRONT, GL_FILL);  
        glColor4f(1.0, 1.0, 1.0, 0.75);

        glBegin(GL_POLYGON);
            glVertex2f( x, y );
            glVertex2f( x, y + h );
            glVertex2f( x + w, y + h );
            glVertex2f( x + w, y );
        glEnd();

        glColor4f(0.0, 0.0, 0.0, 1.0);
        PrintText::printAt(x, y + 0.2 * h, label, false, false, font);
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

void GraphMarker::setPositionX(float x) {
    marker->setLocation(x, marker->getY());
}

void GraphMarker::setPositionY(float y) {
    marker->setLocation(marker->getX(), y);
}