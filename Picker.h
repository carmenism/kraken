#ifndef _PICKER_H
#define _PICKER_H

class Pickable;
class MyQGLWidget;

#include <vector>

class Picker {
public:
    Picker(MyQGLWidget *widget);
    ~Picker() {}

    void pick(std::vector<Pickable *> *allPickables, int x, int y);
private:
    MyQGLWidget *widget;
    Pickable *hovered;
    void setHovered(Pickable *pickable);
};

#endif