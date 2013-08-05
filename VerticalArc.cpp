#include "VerticalArc.h"
#include <math.h>
#include <QtOpenGL>

VerticalArc::VerticalArc(float x, float yA, float yB) : CenteredArc() {
    this->x = x;
    this->yA = yA;
    this->yB = yB;

    arcAngle = M_PI;
    setArcToRight();
}

VerticalArc::~VerticalArc() {

}

void VerticalArc::draw() {
    setCenterLocation();
    CenteredArc::draw();
}

void VerticalArc::setCenterLocation() {
    radius = fabs(yA - yB) / 2;
    y = min(yA, yB) + radius;
}

void VerticalArc::setArcToLeft() {
    startAngle = M_PI / 2.0;
}

void VerticalArc::setArcToRight() {
    startAngle = 3.0 * M_PI / 2.0;
}

bool VerticalArc::arcToRight() {
    return (startAngle > M_PI);
}