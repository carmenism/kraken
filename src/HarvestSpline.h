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
    //void drawFaded();
    virtual void drawSelected();	
    virtual void drawToPick();
	virtual void setPickColor(unsigned char r, unsigned char g, unsigned char b);
	
    void setPointA(Point *p) { pointA = p; }
    Point *getPointA() { return pointA; }

    void setPointB(Point *p) { pointB = p; }
    Point *getPointB() { return pointB; }

    void setControlA(Point *p) { controlA = p; }
    Point *getControlA() { return controlA; }

    void setControlB(Point *p) { controlB = p; }
    Point *getControlB() { return controlB; }
private:	
    Point *pointA, *pointB, *controlA, *controlB;
    Point **middle, **left, **right;
    ChangeSlider *slider;
    SmallMultiple *chart;
	
    int numberPoints;  
	
    void drawSigns();
	void splineConstruct();

	void drawHelper(float alpha);
};

#endif /*HARVESTSPLINE_H_*/