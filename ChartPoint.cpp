#include "ChartPoint.h"
#include "LineChart.h"
#include "Shape.h"
#include "Circle.h"
#include "Square.h"
#include "Triangle.h"
#include "Color.h"
#include "PrintText.h"

ChartPoint::ChartPoint(LineChart *chart, std::string label, float valueX, float valueY, int shape) {
    this->valueX = valueX;
    this->valueY = valueY;
    this->chart = chart;
    this->label = label;// + ": " + toStr(valueY);

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

ChartPoint::~ChartPoint() {
    delete marker;
}

void ChartPoint::calculateLocation() {
    float posX = valueX * chart->getWidth() / chart->getGlobalMaxX();
    float posY = valueY * chart->getHeight() / chart->getGlobalMaxY();

    marker->setLocation(posX, posY);
}

void ChartPoint::draw() {
    marker->draw();
}

void ChartPoint::drawLabel() {
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

        std::string newLabel = label + ": " + toStr(valueY);
        PrintText::printAt(x, y + 0.2 * h, newLabel, false, false, font);
    }
}

void ChartPoint::drawToPick() {
    marker->drawToPick();
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

void ChartPoint::setPickColor(int r, int g, int b) {
    marker->setPickColor(r, g, b);
}

void ChartPoint::setPositionX(float x) {
    marker->setLocation(x, marker->getY());
}

void ChartPoint::setPositionY(float y) {
    marker->setLocation(marker->getX(), y);
}