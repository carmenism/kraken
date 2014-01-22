#include "Animatable.h"
#include <GL/glut.h>
#include <math.h>
#include <QtOpenGL>
#include <iostream>

Animatable::Animatable() {
    animated = false; 

    for(int i = 0; i < 64; i++) {
		for(int j = 0; j < 64; j++) {
			unsigned int grey = (sin(4.0*3.14159*float(j)/64.0)+1.0)*128.0;

			checker[i][j][0] = grey;
			checker[i][j][1] = grey;
			checker[i][j][2] = grey;
			checker[i][j][3] = 125;	
		}
    }

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texName);

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, checker);

	glDisable(GL_TEXTURE_2D);
}

void Animatable::drawSign(float x, float y, float grey, float alpha, bool positive) {
    glPushMatrix();
        glTranslatef(x, y, 0.0);
        drawSign(grey, alpha, positive);
    glPopMatrix();
}

void Animatable::drawSign(float grey, float alpha, bool positive) {
    if (positive) {
        drawPlus(grey, alpha);
    } else {
        drawMinus(grey, alpha);
    }
}

void Animatable::drawPlus(float grey, float alpha) {
    float size = 10; 
    float thickness = 2.0;

    glLineWidth(thickness);
    glColor4f(grey, grey, grey, alpha);

    glBegin(GL_LINES); 
        glVertex2f(size / 2.0, 0.0);	 
        glVertex2f(-size / 2.0, 0.0);

        glVertex2f(0.0, -size / 2.0);
        glVertex2f(0.0, size / 2.0);	
    glEnd();
}

void Animatable::drawMinus(float grey, float alpha) {
    float width = 9;
    float thickness = 3;

    glLineWidth(thickness);
    glColor4f(1, 1, 1, alpha);

    glBegin(GL_LINE_LOOP); 
        glVertex2f(width / 2.0, 0.0);	 
        glVertex2f(-width / 2.0, 0.0);	
    glEnd();
    
    glLineWidth(1.0);
    glColor4f(grey, grey, grey, alpha);
    
    glBegin(GL_LINE_LOOP); 
        glVertex2f(width / 2.0, thickness / 2.0);	 
        glVertex2f(width / 2.0, -thickness / 2.0);	 
        glVertex2f(-width / 2.0, -thickness / 2.0); 
        glVertex2f(-width / 2.0, thickness / 2.0);		
    glEnd();
}