#include "MouseEventItem.h"

MouseEventItem::MouseEventItem() {
    mouseIsHovering = false;
    mouseIsPressing = false;
    active = true;
}

void MouseEventItem::activeOn() { 
    setActive(true);
}

void MouseEventItem::activeOff() {
    setActive(false);
}

void MouseEventItem::setActive(bool a) {
    active = a;
    
    if (!active) {
        mouseIsHovering = false;
        mouseIsPressing = false;
    }
}