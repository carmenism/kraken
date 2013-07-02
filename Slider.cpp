#include <GL/glut.h>
#include <iostream>
#include "Slider.h"
#include "Color.h"

#define HEIGHT 15.0f 
#define CUR_WID	20.0f 
#define BORDER 2.0f 

#define ACTIVE	1
#define INACTIVE 2

Slider::Slider(float x, float y,float length, float start) {
	cornerX = x;
	cornerY = y;
	Len = length + CUR_WID;

	curX = start*length;
	state = INACTIVE;

    mainColor = new Color(0.5, 0.5, 0.5, 1.0);
    cursorColor = new Color(0.5, 0.5, 0.5, 1.0);
    shadowAlpha = 0.75;
    highlightAlpha = 0.75;
}


void Slider::draw() {
	glDisable(GL_DEPTH_TEST);

	glColor4f(mainColor->r, mainColor->g, mainColor->b, mainColor->a);
	glRectf(cornerX,                    cornerY,
            cornerX + Len + 2 * BORDER, cornerY + HEIGHT + 2 * BORDER);

    // Shadow
	glColor4f(0.0, 0.0, 0.0, shadowAlpha);
	glRectf(cornerX,       cornerY, 
            cornerX + Len + 2 * BORDER, cornerY + BORDER);
	glRectf(cornerX + Len + BORDER,     cornerY + BORDER,
            cornerX + Len + 2 * BORDER, cornerY + HEIGHT + 2 * BORDER);
	
    // Highlight
	glColor4f(1.0, 1.0, 1.0, highlightAlpha);
	glRectf(cornerX,          cornerY + BORDER, 
            cornerX + BORDER, cornerY + 2 * BORDER + HEIGHT);
	glRectf(cornerX + BORDER,       cornerY + HEIGHT + BORDER,
            cornerX + BORDER + Len, cornerY + HEIGHT + 2 * BORDER);

	// Cursor
	glColor4f(cursorColor->r, cursorColor->g, cursorColor->b, cursorColor->a);
	glRectf(cornerX + curX,           cornerY + BORDER, 
            cornerX + curX + CUR_WID, cornerY + BORDER + HEIGHT);

    // Cursor shadow
    glColor4f(0.0, 0.0, 0.0, shadowAlpha);
	glRectf(cornerX + curX,           cornerY + BORDER,
            cornerX + curX + CUR_WID, cornerY + 2 * BORDER);
	glRectf(cornerX + curX + CUR_WID - BORDER, cornerY + 2 * BORDER,
            cornerX + curX + CUR_WID,          cornerY + HEIGHT + BORDER);
	
    // Cursor highlight
    glColor4f(1.0, 1.0, 1.0, highlightAlpha);
	glRectf(cornerX + curX,          cornerY + 2 * BORDER, 
            cornerX + curX + BORDER, cornerY + HEIGHT + BORDER);
    glRectf(cornerX + curX + BORDER,           cornerY + HEIGHT,
            cornerX + curX + CUR_WID - BORDER, cornerY + HEIGHT + BORDER);
	
	glEnable(GL_DEPTH_TEST);
}

float Slider::update(float x, float y, bool b) {
	float left;
	float dx;

	left = curX + cornerX - 1;	

	if (!b) {
		state = INACTIVE;
		return curX/(Len/CUR_WID);
	}
	
	if (state == ACTIVE) {
		dx = x-startX;
		curX = startCurX + dx;
		if (curX < 0) curX = 0;
		if (curX> (Len-CUR_WID)) curX = Len - CUR_WID;
	}

	if ((x > left)&& (x < (left+ CUR_WID))&& (y < cornerY + HEIGHT)&&(y >cornerY)) {
		if((state == INACTIVE)&&b) {
			state = ACTIVE;
			startX = x;
			startCurX = curX;
			
            std::cerr << "Curson \n";
		}
	}

	return curX/(Len-CUR_WID);
}