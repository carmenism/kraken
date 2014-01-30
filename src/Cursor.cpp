#include "Cursor.h"

void Cursor::setLocation(float x, float y) {
    setX(x);
    setY(y);
}

float Cursor::getX() {
    return ShadowedRectangle::getX() + width / 2.0;
}

float Cursor::getInnerX() {
    return ShadowedRectangle::getInnerX() + width / 2.0;
}

void Cursor::setX(float x) {
    ShadowedRectangle::setX(x - width / 2.0);
}