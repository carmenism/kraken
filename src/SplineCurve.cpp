#include "SplineCurve.h"
#include "Point.h"
#include "Color.h"
#include <math.h>
#include <GL/glut.h>

extern int timer;

SplineCurve::SplineCurve(Point *pointA, Point *pointB, Point *controlA, Point *controlB) 
: Animatable() {
    numberPoints = 250;
    this->pointA = pointA;
    this->pointB = pointB;
    this->controlA = controlA;
    this->controlB = controlB;

    middle = new Point*[numberPoints];
    left = new Point*[numberPoints];
    right = new Point*[numberPoints];

    for (int i = 0; i < numberPoints; i++) {
        middle[i] = new Point(0, 0);
        left[i] = new Point(0, 0);
        right[i] = new Point(0, 0);
    }

    width = 5;
}

SplineCurve::~SplineCurve() {
    delete pointA;
    delete pointB;
    delete controlA;
    delete controlB;
    delete[] middle;
    delete[] left;
    delete[] right;
}

void SplineCurve::construct() {
	float dx = pointB->x - pointA->x; 
	float dy = pointB->y - pointA->y; 
	float length = sqrt(dx * dx + dy * dy);

	float dxA = (controlA->x - pointA->x) * 3.0f;
	float dyA = (controlA->y - pointA->y) * 3.0f;

	float dxB = (pointB->x - controlB->x) * 3.0f;
	float dyB = (pointB->y - controlB->y) * 3.0f;

	_ax = pointA->x;
	_bx = dxA;

	_cx = 3.0f * (pointB->x - pointA->x) - 2.0f * dxA - dxB;
	_dx = 2.0f * (pointA->x - pointB->x) + dxA + dxB;

	_ay = pointA->y;
	_by = dyA;

	_cy = 3.0 * (pointB->y - pointA->y) - 2.0 * dyA - dyB;
	_dy = 2.0 * (pointA->y - pointB->y) + dyA + dyB;
		
	for (int i = 0; i < numberPoints; i++) {
		float u = float(i) / float(numberPoints - 1);

		middle[i]->setX(_ax + _bx * u + _cx * u * u + _dx * u * u * u);
		middle[i]->setY(_ay + _by * u + _cy * u * u + _dy * u * u * u);
	}

    for (int i = 1; i < numberPoints; i++) {
		float w = 0.03 + width * float(i) / numberPoints;

		dx = middle[i]->x - middle[i - 1]->x;
		dy = middle[i]->y - middle[i - 1]->y;
        length = sqrt(dx * dx + dy * dy);

		dx = w * dx / length;//w*dx/len;  // the width.
		dy = w * dy / length;//w*dy/len;

        left[i]->setValues(dy, -dx);
        right[i]->setValues(-dy, dx);

        if (i == 1) {		
            left[0]->setValues(dy, -dx);
            right[0]->setValues(-dy, dx);
		}
	}
}

void SplineCurve::draw() {
    if (fabs(width) < 1) {
        return;
    }
    	
    float timeval;
	timeval = float(timer % numberPoints)/float(numberPoints);

	float tc;

    if (animated) {
	    glEnable(GL_TEXTURE_2D);
    } 

    glPolygonMode(GL_FRONT, GL_FILL); 

	glBegin(GL_QUAD_STRIP); 
	for (int i = 0; i < numberPoints; i++) {
        tc = 1.5*((numberPoints - float(i))/numberPoints+timeval);

		glColor4f(color->r, color->g, color->b, color->a);

        if (animated) {
            glTexCoord2f(tc,0.3);
        }

		glVertex2f(middle[i]->x + left[i]->x, middle[i]->y + left[i]->y);
		
        if (animated) {
            glTexCoord2f(tc,0.3);
        }

        glVertex2f(middle[i]->x, middle[i]->y);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < numberPoints; i++) {
        tc = 1.5*((numberPoints - float(i))/numberPoints+timeval);

		glColor4f(color->r, color->g, color->b, color->a);
        
        if (animated) {
            glTexCoord2f(tc,0.3);
        }

		glVertex2f(middle[i]->x, middle[i]->y);
        
        if (animated) {
            glTexCoord2f(tc,0.3);
        }

		glVertex2f(middle[i]->x + right[i]->x, middle[i]->y + right[i]->y);
	}
	glEnd();   

    if (animated) {
        glDisable(GL_TEXTURE_2D);
        drawSigns();
    }
}

void SplineCurve::drawSigns() {
    glColor3f(0.0,0.0,0.0);
    for (int i = 0; i < 5; i++) {
        int t = numberPoints-(timer+numberPoints/5*i)%numberPoints-1;
        glPushMatrix();
            glTranslatef(middle[numberPoints - t - 1]->x, middle[numberPoints - t - 1]->y, 0.0);
            
            Animatable::drawSigns(width < 0);
        glPopMatrix();
    }
}