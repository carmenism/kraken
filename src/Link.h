#ifndef _LINK_H
#define _LINK_H

#include "Point.h"
#include "Pickable.h"
#include "Animatable.h"

class Link : public Point, public Pickable, public Animatable {
public:
	Link() : Point(0, 0) { }
    ~Link() { }

	virtual void draw() = 0;
    virtual void drawToPick() = 0;
    virtual void drawSelected() = 0;
	
    bool getDisplayDynamically() { return displayDynamically; }
    void setDisplayDynamically(bool d) { displayDynamically = d; }
    void displayDynamicallyOn() { displayDynamically = true; }
    void displayDynamicallyOff() { displayDynamically = false; }
	
    Color *getColor() { return color; }
    void setColor(Color *c) { color = c; }
	
    float getThickness() { return thickness; }
    void setThickness(float t) { thickness = t; }

    float getHighlightThickness() { return highlightThickness; }
    void setHighlightThickness(float t) { highlightThickness = t; }
protected:
    bool displayDynamically;
	Color *color;
	float thickness, highlightThickness;
};

#endif /* _LINK_H */