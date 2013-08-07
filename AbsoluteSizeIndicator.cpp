#include "AbsoluteSizeIndicator.h"
#include "Shape.h"
#include "Color.h"

AbsoluteSizeIndicator::AbsoluteSizeIndicator(float valueX, float valueY) {
    this->valueX = valueX;
    this->valueY = valueY;
    Shape::getBorderColor()->a = 0.25;
    Shape::getFillColor()->a = 0.25;
}

AbsoluteSizeIndicator::~AbsoluteSizeIndicator() {

}

