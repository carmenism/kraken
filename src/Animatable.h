#ifndef _ANIMATABLE_H
#define _ANIMATABLE_H

class Animatable {
public:
    Animatable() { animated = false; }
    ~Animatable() {}

    bool isAnimated() { return animated; }
    bool getAnimated() { return animated; }
    void setAnimated(bool a) { animated = a; }
    void animatedOn() { animated = true; }
    void animatedOff() { animated = false; }
protected:
    bool animated;
    
    void drawSigns(bool positive);
    void drawPlus();
    void drawMinus();
};

#endif /* _ANIMATABLE_H */