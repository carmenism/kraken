#include "CenteredArc.h"
#include "Color.h"
#include "Triangle.h"
#include <math.h>
#include <GL/glut.h>
#include <QtOpenGL>

extern int timer;

float *CenteredArc::xArc = new float[NUM_SEGMENTS];
float *CenteredArc::yArc = new float[NUM_SEGMENTS];

CenteredArc::CenteredArc() : Point(0, 0), Pickable() {
    radius = 10;
    thickness = 1;
    highlightThickness = 4;
    highlightColor = &Color::white;

    arcToRight = true;
    displayDynamically = false;
    animated = false;
    fadingAlpha = true;

    startAlpha = 0.6;
    finalAlpha = 0.15;
    signAlpha = 0.75;

    //CW_CODE
	
    for(int i = 0; i < NUM_SEGMENTS; i++) {
		double a = M_PI * float(i) / (NUM_SEGMENTS - 1);
		xArc[i] = sin(a);
		yArc[i] = -cos(a);
	}

    
    arrowA = new Triangle();
    arrowA->setBorderColor(new Color(0, 0, 0, .95));
    arrowA->setFillColor(new Color(1, 1, 1, .65));
    arrowA->setSize(10, 10);

    arrowB = new Triangle();
    arrowB->setBorderColor(new Color(0, 0, 0, .95));
    arrowB->setFillColor(new Color(1, 1, 1, .65));
    arrowB->setSize(10, 10);
    
    arrowMiddle = new Triangle();
    arrowMiddle->setBorderColor(new Color(0, 0, 0, .95));
    arrowMiddle->setFillColor(new Color(1, 1, 1, .65));
    arrowMiddle->setSize(10, 10);
}

CenteredArc::~CenteredArc() {
    delete color;
    delete highlightColor;
    delete arrowA;
    delete arrowB;
    delete arrowMiddle;
}

CenteredArc::CenteredArc(float radius, float centerX, float centerY) 
: Point(centerX, centerY) {
    this->radius = radius;
}

void CenteredArc::draw() {
    glPushMatrix();
        glTranslatef(x, y, 0);

        float startA = startAlpha;
        float finalA = finalAlpha;               

        if (!fadingAlpha) {
            startA = color->a;
            finalA = color->a;
        }

        if (!arcToRight) {
            glRotatef(180.0,0.0,0.0,1.0);
        }

        if (thickness != 0) {
            if (selected) {
                drawHighlight();
            }

            if (fabs(thickness) < 3) {
                drawLineArc();
            } else {
                drawPolygonArc();
            }

            drawTriangles();
        }
    glPopMatrix();
}

void CenteredArc::drawToPick() {
    glPushMatrix();
        glTranslatef(x, y, 0);

        if (!arcToRight) {
            glPushMatrix();
            glRotatef(180.0,0.0,0.0,1.0);
        }

        if (fabs(thickness) < 3) {
            drawToPickAsLineStrips();
        } else {
            drawToPickAsPolygons();
        }
        
        if (!arcToRight) {
            glPopMatrix();
        }
    glPopMatrix();
}

void CenteredArc::drawToPickAsLineStrips() {
    glEnable(GL_POLYGON_SMOOTH);
        
    glLineWidth(thickness);
    glColor3ub(pickR, pickG, pickB);

    glBegin(GL_LINE_STRIP);
        
    for(int i = 0; i < NUM_SEGMENTS; i++) { 
        glVertex2f(xArc[i] * radius, yArc[i] * radius);
    }

    glEnd();

    glLineWidth(1);
    glDisable(GL_POLYGON_SMOOTH);
}

void CenteredArc::drawToPickAsPolygons() {
    float radiusOuter = radius + thickness / 2;
    float radiusInner = radius - thickness / 2;
    
    glPolygonMode(GL_FRONT, GL_FILL);

    glBegin(GL_TRIANGLE_STRIP);
    glColor3ub(pickR, pickG, pickB);

    for (int i = 0; i < NUM_SEGMENTS; i++) { 
		glVertex2f(xArc[i] * radiusOuter, yArc[i] * radiusOuter);        
        glVertex2f(xArc[i] * radiusInner, yArc[i] * radiusInner);
    }

    glEnd();
}

void CenteredArc::drawSelected() {
    if (selected) {
        float oldStart = startAlpha;
        float oldFinal = finalAlpha;
        float oldSignAlpha = signAlpha;
        startAlpha = 0.4;
        finalAlpha = 0.0;
        signAlpha = 1.0;
        draw();
        startAlpha = oldStart;
        finalAlpha = oldFinal;
        signAlpha = oldSignAlpha;
    }
}

void CenteredArc::drawLineArc() {
    glEnable(GL_POLYGON_SMOOTH);
    
    float diff = startAlpha - finalAlpha;
    float alphaStep = diff / NUM_SEGMENTS;

    float mult = 1;

    if (!arcToRight) {
        mult = -1;
    }

    glLineWidth(thickness);

    glBegin(GL_LINE_STRIP);

    for(int i = 0; i < NUM_SEGMENTS; i++) { 
        glColor4f( color->r, color->g, color->b, finalAlpha + i * alphaStep );
        glVertex2f(xArc[i] * radius, yArc[i] * radius);
    }

    glEnd();

    glLineWidth(1);
    glDisable(GL_POLYGON_SMOOTH);
}

void CenteredArc::drawPolygonArc() {
    float radiusOuter = radius + thickness / 2;
    float radiusInner = radius - thickness / 2;

    float diff = startAlpha - finalAlpha;
    float alphaStep = diff / NUM_SEGMENTS;
    
    //CW_CODE

	float timeval;
	timeval = 1.0 - float(timer % NUM_SEGMENTS)/float(NUM_SEGMENTS);

	float tc;

    if (displayDynamically && animated) {
	    glEnable(GL_TEXTURE_2D);
    }   
 
    glPolygonMode(GL_FRONT, GL_FILL);

	glBegin(GL_TRIANGLE_STRIP);
    for(int i = 0; i < NUM_SEGMENTS; i++) { 
        glColor4f( color->r, color->g, color->b, finalAlpha + i * alphaStep);

        tc = 1.5*((NUM_SEGMENTS - float(i))/NUM_SEGMENTS+timeval);

        //CW_CODE 
        //*
        if (displayDynamically && animated) {
            glTexCoord2f(tc, 0.3);
        }

        glVertex2f(xArc[i]*radiusOuter,yArc[i]*radiusOuter);

        if (displayDynamically && animated) {
            glTexCoord2f(tc,0.3);
        }
        
        glVertex2f(xArc[i]*radiusInner, yArc[i]*radiusInner);
    }
    glEnd();

    if (displayDynamically && animated) {
        glDisable(GL_TEXTURE_2D);
    }
}

void CenteredArc::drawSigns() {
	float grey = 0.2;
	float signA = signAlpha;
	
	if (selected) {
		grey = 0;
		signAlpha = 1.0;
	}

    glPushMatrix();
        glTranslatef(x, y, 0);

        if (!arcToRight) {
            glRotatef(180.0,0.0,0.0,1.0);
        }

        if (displayDynamically && animated) {
            for (int i = 0; i < 5; i++) {
                int t = NUM_SEGMENTS-(timer+NUM_SEGMENTS/5*i)%NUM_SEGMENTS-1;
                
                drawSign(xArc[t]*radius, yArc[t]*radius, grey, signA, thickness < 0);
            }
        } else {
            int oneSixth = NUM_SEGMENTS / 6;
            drawSign(xArc[oneSixth]*radius, yArc[oneSixth]*radius, grey, signA, thickness < 0);
            drawSign(xArc[2 * oneSixth]*radius, yArc[2 * oneSixth]*radius, grey, signA, thickness < 0);
            drawSign(xArc[4 * oneSixth]*radius, yArc[4 * oneSixth]*radius, grey, signA, thickness < 0);
            drawSign(xArc[5 * oneSixth]*radius, yArc[5 * oneSixth]*radius, grey, signA, thickness < 0);
        }
    glPopMatrix();
}

void CenteredArc::drawHighlight() {    
    float radiusOuter = radius + fabs(thickness / 2);
    float radiusInner = radius - fabs(thickness / 2);
    float highlightOuter = radiusOuter + highlightThickness / 2;
    float highlightInner = radiusInner - highlightThickness / 2;

    glLineWidth(highlightThickness);
    glColor4f( highlightColor->r, highlightColor->g, highlightColor->b, highlightColor->a );   
    
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < NUM_SEGMENTS; i++) { 
        glVertex2f(xArc[i] * highlightOuter, yArc[i] * highlightOuter);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < NUM_SEGMENTS; i++) { 
        glVertex2f(xArc[i] * highlightInner, yArc[i] * highlightInner);
    }
    glEnd();

    glLineWidth(1);
}

void CenteredArc::setArcToLeft() {
    arcToRight = false;
}

void CenteredArc::setArcToRight() {
    arcToRight = true;
}

bool CenteredArc::isArcToRight() {
    return arcToRight;
}

void CenteredArc::drawTriangles() {
    if (selected) {
        arrowA->draw();
        arrowB->draw();
    } else {
        arrowMiddle->draw();
    }
}

void CenteredArc::positionTriangles() {
    Color *c = this->getColor();

    arrowA->setBorderColor(c);
    arrowB->setBorderColor(c);    
    arrowMiddle->setBorderColor(c);

    arrowA->setBorderWidth(2);
    arrowB->setBorderWidth(2);
    arrowMiddle->setBorderWidth(1.5);

    float size = max(6, fabs(getThickness() / 2));

    int oneFourth = NUM_SEGMENTS / 4;

    arrowA->setLocation(xArc[oneFourth]*radius, yArc[oneFourth]*radius);
    arrowA->setSize(size, size * 2);
    arrowA->setRotation(135);

    arrowB->setLocation(xArc[3 * oneFourth]*radius, yArc[3 * oneFourth]*radius);
    arrowB->setSize(size, size * 2);
    arrowB->setRotation(225);

    arrowMiddle->setLocation(xArc[2 * oneFourth]*radius, yArc[2 * oneFourth]*radius);
    arrowMiddle->setSize(size, size * 2);
    arrowMiddle->setRotation(180);    
}
