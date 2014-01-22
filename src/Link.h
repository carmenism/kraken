#ifndef _LINK_H
#define _LINK_H

#include "Point.h"
#include "Pickable.h"
#include "Animatable.h"

class Link : public Point, public Pickable, public Animatable {
public:
	Link() : Point(0, 0) { }
    ~Link() { }

    virtual void drawToPick() = 0;
    virtual void drawSelected() = 0;
protected:

};

#endif /* _LINK_H */