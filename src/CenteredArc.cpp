#include "CenteredArc.h"
#include "Color.h"
#include <math.h>

extern int timer;

CenteredArc::CenteredArc() : Point(0, 0), Pickable() {
    radius = 10;
    thickness = 1;
    highlightThickness = 4;
    highlightColor = &Color::white;
    num_segments = 360;//CW_CODE
    arcToRight = true;

    animated = false;

    fadingAlpha = false;
    startAlpha = 0.6;
    finalAlpha = 0.15;

    //CW_CODE
	unsigned int grey;
	
	xArc = new float[num_segments];
	yArc = new float[num_segments];
    for(int i = 0; i < num_segments; i++) {
		double a = M_PI * float(i) / (num_segments - 1);
		xArc[i] = sin(a);
		yArc[i] = -cos(a);
	}

    for(int i = 0; i < 64; i++) {
		for(int j = 0; j < 64; j++) {
			grey = (sin(4.0*3.14159*float(j)/64.0)+1.0)*128.0;

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64,64,0, GL_RGBA, GL_UNSIGNED_BYTE, checker);

	glDisable(GL_TEXTURE_2D);
}

CenteredArc::~CenteredArc() {
    delete color;
    delete highlightColor;
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

        if (fabs(thickness) < 3) {
            drawLineArc();
        } else {
            drawPolygonArc();
        }

        if (selected) {
            drawHighlight();
        }        
    glPopMatrix();
}

void CenteredArc::drawToPick() {
    glPushMatrix();
        glTranslatef(x, y, 0);

        if (fabs(thickness) < 3) {
            drawToPickAsLineStrips();
        } else {
            drawToPickAsPolygons();
        }
        
    glPopMatrix();
}

void CenteredArc::drawToPickAsLineStrips() {
    float mult = 1;

    if (!arcToRight) {
        mult = -1;
    }

    glEnable(GL_POLYGON_SMOOTH);
        
    glLineWidth(thickness);
    glColor3ub(pickR, pickG, pickB);

    glBegin(GL_LINE_STRIP);
        
    for(int ii = 0; ii < num_segments; ii++) { 
        glVertex2f(mult * xArc[ii] * radius, yArc[ii] * radius);
    }

    glEnd();

    glLineWidth(1);
    glDisable(GL_POLYGON_SMOOTH);
}

void CenteredArc::drawToPickAsPolygons() {
    float radiusOuter = radius + thickness / 2;
    float radiusInner = radius - thickness / 2;
    
    float mult = 1;

    if (!arcToRight) {
        mult = -1;
    }

    glPolygonMode(GL_FRONT, GL_FILL);

    glBegin(GL_TRIANGLE_STRIP);
    glColor3ub(pickR, pickG, pickB);

    for(int ii = 0; ii < num_segments; ii++) { 
		glVertex2f(mult * xArc[ii]*radiusOuter,yArc[ii]*radiusOuter);        
        glVertex2f(mult * xArc[ii]*radiusInner, yArc[ii]*radiusInner);
    }

    glEnd();
}

void CenteredArc::drawSelected() {
    if (selected) {
        float oldStart = startAlpha;
        float oldFinal = finalAlpha;
        startAlpha = 0.4;
        finalAlpha = 0.0;
        draw();
        startAlpha = oldStart;
        finalAlpha = oldFinal;
    }
}

void CenteredArc::drawLineArc() {
    glEnable(GL_POLYGON_SMOOTH);
    
    float diff = startAlpha - finalAlpha;
    float alphaStep = diff / num_segments;

    float mult = 1;

    if (!arcToRight) {
        mult = -1;
    }

    glLineWidth(thickness);

    glBegin(GL_LINE_STRIP);

    for(int ii = 0; ii < num_segments; ii++) { 
        glColor4f( color->r, color->g, color->b, startAlpha - ii * alphaStep );
        glVertex2f(mult * xArc[ii] * radius, yArc[ii] * radius);
    }

    glEnd();

    glLineWidth(1);
    glDisable(GL_POLYGON_SMOOTH);
}

void CenteredArc::drawPolygonArc() {
    float radiusOuter = radius + thickness / 2;
    float radiusInner = radius - thickness / 2;

    float diff = startAlpha - finalAlpha;
    float alphaStep = diff / num_segments;
    
    //CW_CODE

	float timeval;
	timeval = 1.0 - float(timer%num_segments)/float(num_segments);

	float tc;

    if (animated) {
	    glEnable(GL_TEXTURE_2D);
    }

    float mult = 1;

    if (!arcToRight) {
        mult = -1;
    }   
 
    glPolygonMode(GL_FRONT, GL_FILL);

	glBegin(GL_TRIANGLE_STRIP);
    for(int ii = 0; ii < num_segments; ii++) { 
        glColor4f( color->r, color->g, color->b, startAlpha - ii * alphaStep);

        tc = 1.5*(float(ii)/num_segments+timeval);

        //CW_CODE 
        //*
        if (animated) {
            glTexCoord2f(tc, 0.3);
        }

        glVertex2f(mult * xArc[ii]*radiusOuter,yArc[ii]*radiusOuter);

        if (animated) {
            glTexCoord2f(tc,0.3);
        }
        
        glVertex2f(mult * xArc[ii]*radiusInner, yArc[ii]*radiusInner);
    }
    glEnd();

    if (animated) {
        glDisable(GL_TEXTURE_2D);
    }

    // CW_CODE now draw +s

    if (animated) {
        glLineWidth(2.0);
        int t;

        glColor3f(0.0,0.0,0.0);
        for (int i = 0; i < 5; i++) {
            t = num_segments-(timer+num_segments/5*i)%num_segments-1;
            glPushMatrix();
                if(!arcToRight) glRotatef(180.0,0.0,0.0,1.0);

                glTranslatef(xArc[t]*radius,yArc[t]*radius,0.0);
                glBegin(GL_LINES); 
                    glVertex2f(5.0,0.0);	 
                    glVertex2f(-5.0,0.0);

                    if(thickness < 0) {
                        glVertex2f(0.0,-5.0);
                        glVertex2f(0.0,5.0);	
                    }		
                glEnd();
            glPopMatrix();
        }
    }
}

void CenteredArc::drawHighlight() {    
    float radiusOuter = radius + thickness / 2;
    float radiusInner = radius - thickness / 2;
    float highlightOuter = radiusOuter + highlightThickness / 2;
    float highlightInner = radiusInner - highlightThickness / 2;

    float mult = 1;

    if (!arcToRight) {
        mult = -1;
    }

    glLineWidth(highlightThickness);
    glColor4f( highlightColor->r, highlightColor->g, highlightColor->b, highlightColor->a );   
    
    glBegin(GL_LINE_STRIP);
    for(int ii = 0; ii < num_segments; ii++) { 
        glVertex2f(mult * xArc[ii] * highlightOuter, yArc[ii] * highlightOuter);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for(int ii = 0; ii < num_segments; ii++) { 
        glVertex2f(mult * xArc[ii] * highlightInner, yArc[ii] * highlightInner);
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