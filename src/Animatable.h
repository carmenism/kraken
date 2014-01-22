#ifndef _ANIMATABLE_H
#define _ANIMATABLE_H

class Animatable {
public:
    Animatable();
    ~Animatable() {}

    bool isAnimated() { return animated; }
    bool getAnimated() { return animated; }
    void setAnimated(bool a) { animated = a; }
    void animatedOn() { animated = true; }
    void animatedOff() { animated = false; }
protected:
    bool animated;
    
    void drawSign(float x, float y, float grey, float alpha, bool positive);
private:
    void drawSign(float grey, float alpha, bool positive);
    void drawPlus(float grey, float alpha);
    void drawMinus(float grey, float alpha);
    
	unsigned char checker[64][64][4];
	unsigned int texName;
};

#endif /* _ANIMATABLE_H */