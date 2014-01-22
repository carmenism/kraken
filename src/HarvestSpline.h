#ifndef HARVESTSPLINE_H_
#define HARVESTSPLINE_H_

class SmallMultiple;
class ChangeSlider;
class Color;

#include "Link.h"

class HarvestSpline : public Link {
public:
    HarvestSpline(ChangeSlider *slider, SmallMultiple *chart);
    ~HarvestSpline();

    virtual void construct();
    void draw();
    virtual void drawToPick();
	virtual void drawSelected();
	
    void setPointA(Point *p) { pointA = p; }
    Point *getPointA() { return pointA; }

    void setPointB(Point *p) { pointB = p; }
    Point *getPointB() { return pointB; }

    void setControlA(Point *p) { controlA = p; }
    Point *getControlA() { return controlA; }

    void setControlB(Point *p) { controlB = p; }
    Point *getControlB() { return controlB; }

    void setColor(Color *c) { color = c; }
    Color *getColor() { return color; }

    float getWidth() { return width; }
    void setWidth(float w) { width = w; }
private:	
    Color *color;
    Point *pointA, *pointB, *controlA, *controlB;
    Point **middle, **left, **right;
    ChangeSlider *slider;
    SmallMultiple *chart;
	
    int numberPoints;
    float width;   
	
    void drawSigns();
	void splineConstruct();
};

#endif /*HARVESTSPLINE_H_*/