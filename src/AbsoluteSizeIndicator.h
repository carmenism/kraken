#ifndef ABSOLUTESIZEINDICATOR_H_
#define ABSOLUTESIZEINDICATOR_H_

#include "Circle.h"
#include <string>

#define MAX_SIZE 600000

class AbsoluteSizeIndicator : public Circle, public Pickable {
public:
    AbsoluteSizeIndicator(std::string label, float valueX, float valueY);
    ~AbsoluteSizeIndicator();

    void setValueX(float valueX) { this->valueX = valueX; } 
    float getValueX() { return valueX; }

    void setValueY(float valueY) { this->valueY = valueY; }
    float getValueY() { return valueY; }

    virtual void drawToPick();
    virtual void drawSelected();
private:
    std::string label;
    float valueX, valueY;  
};

#endif /*ABSOLUTESIZEINDICATOR_H_*/