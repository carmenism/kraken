#ifndef ABSOLUTESIZEINDICATOR_H_
#define ABSOLUTESIZEINDICATOR_H_

#include "Circle.h"

#define MAX_SIZE 600000

class AbsoluteSizeIndicator : public Circle {
public:
    AbsoluteSizeIndicator(float valueX, float valueY);
    ~AbsoluteSizeIndicator();

    void setValueX(float valueX) { this->valueX = valueX; } 
    float getValueX() { return valueX; }

    void setValueY(float valueY) { this->valueY = valueY; }
    float getValueY() { return valueY; }
private:
    float valueX, valueY;  
};

#endif /*ABSOLUTESIZEINDICATOR_H_*/