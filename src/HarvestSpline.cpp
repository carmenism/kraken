#include "HarvestSpline.h"
#include "Color.h"
#include "Point.h"
#include "ChangeSlider.h"
#include "SmallMultiple.h"
#include <math.h>
#include <GL/glut.h>

extern int timer;

HarvestSpline::HarvestSpline(ChangeSlider *slider, SmallMultiple *chart)
: Link() {
	numberPoints = 250;
	pointA = new Point(0, 0);
	pointB = new Point(0, 0);
	controlA = new Point(0, 0);
	controlB = new Point(0, 0);

    this->slider = slider;
    this->chart = chart;

	middle = new Point*[numberPoints];
    left = new Point*[numberPoints];
    right = new Point*[numberPoints];

    for (int i = 0; i < numberPoints; i++) {
        middle[i] = new Point(0, 0);
        left[i] = new Point(0, 0);
        right[i] = new Point(0, 0);
    }

    width = 5;
	
    setColor(new Color(0.9, 0.9, 0, 0.45));
}

HarvestSpline::~HarvestSpline() {
    delete pointA;
    delete pointB;
    delete controlA;
    delete controlB;
    delete[] middle;
    delete[] left;
    delete[] right;
}

void HarvestSpline::construct() {
    float currentValue = slider->getValue();
    float previousValue = slider->getPreviousValue();
    float newWidth = (currentValue - previousValue) * 8;

    float sliderX = slider->getX() + slider->getWidth();
    float sliderY = slider->getY() + slider->getHeight() / 2.0;
    
    float chartX = chart->getXLocation();
    float chartY = chart->getYLocation() + chart->getHeight() / 2.0;

    if (getWidth() != newWidth 
        || pointA->x != sliderX || pointA->y != sliderY
        || pointB->x != chartX || pointB->y != chartY) {
        setWidth(newWidth);
            
        pointA->setValues(sliderX, sliderY);
        pointB->setValues(chartX, chartY);
        
        float diffX = chartX - sliderX;

        controlA->setValues(sliderX + diffX / 3.0, sliderY);
        controlB->setValues(sliderX + 2 * diffX / 3.0, chartY);

        splineConstruct();
    }
}

void HarvestSpline::splineConstruct() {
	float dx = pointB->x - pointA->x; 
	float dy = pointB->y - pointA->y; 
	float length = sqrt(dx * dx + dy * dy);

	float dxA = (controlA->x - pointA->x) * 3.0f;
	float dyA = (controlA->y - pointA->y) * 3.0f;

	float dxB = (pointB->x - controlB->x) * 3.0f;
	float dyB = (pointB->y - controlB->y) * 3.0f;

	float _ax = pointA->x;
	float _bx = dxA;
	float _cx = 3.0f * (pointB->x - pointA->x) - 2.0f * dxA - dxB;
	float _dx = 2.0f * (pointA->x - pointB->x) + dxA + dxB;

	float _ay = pointA->y;
	float _by = dyA;
	float _cy = 3.0 * (pointB->y - pointA->y) - 2.0 * dyA - dyB;
	float _dy = 2.0 * (pointA->y - pointB->y) + dyA + dyB;
		
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

void HarvestSpline::draw() {
    if (fabs(width) < 1) {
        return;
    }
    	
    float timeval;
	timeval = float(timer % numberPoints)/float(numberPoints);

    if (animated) {
	    glEnable(GL_TEXTURE_2D);
    } 

    glPolygonMode(GL_FRONT, GL_FILL); 
    glColor4f(color->r, color->g, color->b, color->a);

	glBegin(GL_QUAD_STRIP); 
	for (int i = 0; i < numberPoints; i++) {
        float tc = 1.5*((numberPoints - float(i))/numberPoints+timeval);

        if (animated) {
            glTexCoord2f(tc,0.3);
        }

		glVertex2f(middle[i]->x + left[i]->x, middle[i]->y + left[i]->y);
		
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

void HarvestSpline::drawToPick() {
    if (fabs(width) < 1) {
        return;
    }

    glPolygonMode(GL_FRONT, GL_FILL); 
	glBegin(GL_QUAD_STRIP); 
	glColor3ub(pickR, pickG, pickB);
	
	for (int i = 0; i < numberPoints; i++) {
		glVertex2f(middle[i]->x + left[i]->x, middle[i]->y + left[i]->y);
		glVertex2f(middle[i]->x + right[i]->x, middle[i]->y + right[i]->y);
	}

	glEnd();
}

void HarvestSpline::drawSigns() {
    glColor3f(0.0,0.0,0.0);
    for (int i = 0; i < 5; i++) {
        int t = numberPoints-(timer+numberPoints/5*i)%numberPoints-1;
            
        drawSign(middle[numberPoints - t - 1]->x, middle[numberPoints - t - 1]->y, 0.2, 1.0, width < 0);
    }
}

void HarvestSpline::drawSelected() {

}