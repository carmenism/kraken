#ifndef MOUSEEVENTITEM_H_
#define MOUSEEVENTITEM_H_

class MouseEventItem {
public:
    MouseEventItem();
    ~MouseEventItem() {}
    
    virtual bool mouseMoved(float x, float y) = 0;
    virtual bool mousePressed(float x, float y) = 0;
    virtual bool mouseReleased(float x, float y) = 0;

    bool isActive() { return active; }
    void activeOn();
    void activeOff();
    void setActive(bool a);
protected:
    bool mouseIsHovering;
    bool mouseIsPressing;
    bool active;
};

#endif /*MOUSEEVENTITEM_H_*/