#ifndef _PICKABLE_H
#define _PICKABLE_H

class Pickable {
public:
    Pickable() { selected = false; }
    ~Pickable() {}

    bool getSelected() { return selected; }
    void setSelected(bool s) { selected = s; }
    void selectedOn() { selected = true; }
    void selectedOff() { selected = false; }
    
    float getMouseX() { return mouseX; }
    void setMouseX(float x) { mouseX = x; }

    float getMouseY() { return mouseY; }
    void setMouseY(float y) { mouseY = y; }

    virtual void setPickColor(unsigned char r, unsigned char g, unsigned char b)
        { pickR = r; pickG = g; pickB = b; }

    virtual void drawToPick() = 0;
    virtual void drawSelected() = 0;
protected:
    bool selected;
    unsigned char pickR, pickG, pickB;
    float mouseX, mouseY;
};

#endif /* _PICKABLE_H */