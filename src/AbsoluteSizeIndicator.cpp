#include "AbsoluteSizeIndicator.h"
#include "Shape.h"
#include "Color.h"
#include "PrintText.h"

AbsoluteSizeIndicator::AbsoluteSizeIndicator(std::string label, float valueX, float valueY) {
    this->label = label;
    this->valueX = valueX;
    this->valueY = valueY;
    Shape::getBorderColor()->a = 0.25;
    Shape::getFillColor()->a = 0.25;
}

AbsoluteSizeIndicator::~AbsoluteSizeIndicator() {

}

void AbsoluteSizeIndicator::drawSelected() {
    if (selected) {
        glColor4f(0, 0, 0, 1);
        std::string newLabel = label + ", " + toStr(valueY) + " Mt (Year " + toStr(valueX) + ")";
        PrintText::drawStrokeText(newLabel, mouseX + 10, mouseY - 10, 12, HORIZ_LEFT, VERT_TOP, true);
        getBorderColor()->a = 0.5;
        getFillColor()->a = 0.5;
    } else {
        Shape::getBorderColor()->a = 0.25;
        Shape::getFillColor()->a = 0.25;
    }
}

void AbsoluteSizeIndicator::drawToPick() {
    Shape::setPickColor(Pickable::pickR, Pickable::pickG, Pickable::pickB);
    Shape::drawToPick();
}