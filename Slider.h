#ifndef SLIDER_H_
#define SLIDER_H_

// a slider that works in the range 0..1.0 in pixel coordinates
class Color;

#define ACTIVE	1
#define INACTIVE 2

class Slider
{
public:
	Slider(float xcorner, float ycorner, float length, float start);

	// specify the lower left corner, the slider length
	// and the start value between 0 and 1.0
    
	void draw();

	bool mouseMoved(float x, float y);
    bool mousePressed(float x, float y);
    bool mouseReleased();
private:
	int state;
	int selpos;
	float cornerX, cornerY, Len;
	float curX, startCurX;
	float startX;

    Color *mainColor;
    Color *cursorColor;
    float shadowAlpha;
    float highlightAlpha;

    bool pointInCursor(float x, float y);
};

#endif /* SLIDER_H_ */