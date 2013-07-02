#ifndef SLIDER_H_
#define SLIDER_H_

// a slider that works in the range 0..1.0 in pixel coordinates
class Color;

class Slider
{
public:
	Slider(float xcorner, float ycorner, float length, float start);

	// specify the lower left corner, the slider length
	// and the start value between 0 and 1.0
	void draw();
	float update(float x, float y, bool b);

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
};

#endif /* SLIDER_H_ */