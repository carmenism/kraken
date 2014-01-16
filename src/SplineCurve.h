#ifndef SPLINECURVE_H_
#define SPLINECURVE_H_

class Color;
class Point;

class SplineCurve
{
public:
    SplineCurve(int n, Point *pointA, Point *pointB, Point *controlA, Point *controlB);
    ~SplineCurve();

    virtual void construct();
    void draw();

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
protected:
    Color *color;
    int numberPoints;
    Point *pointA, *pointB;
    Point *controlA, *controlB;
    Point **middle, **left, **right;
    float width;
    
	float _ax, _bx, _cx, _dx, _ay, _by, _cy, _dy;
};

#endif /*SPLINECURVE_H_*/