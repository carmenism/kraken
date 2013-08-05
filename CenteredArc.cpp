#include "CenteredArc.h"
#include "Color.h"
#include <math.h>

CenteredArc::CenteredArc() : Point(0, 0), Pickable() {
    radius = 10;
    thickness = 1;
    startAngle = 3.0 * M_PI / 2.0;
    arcAngle = M_PI;
    color = new Color(0, 0, 0, 0.25);
    quadratic = gluNewQuadric();

    fadingAlpha = false;
    startAlpha = 0.6;
    finalAlpha = 0.15;
}

CenteredArc::~CenteredArc() {

}

CenteredArc::CenteredArc(float radius, float centerX, float centerY, float startAngle, float arcAngle) 
: Point(centerX, centerY) {
    this->radius = radius;
    this->startAngle = startAngle;
    this->arcAngle = arcAngle;
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

        if (thickness < 5) {
            drawLineArc(radius, thickness, startAngle, arcAngle, color, startA, finalA);
        } else {
            drawPolygonArc(radius, thickness, startAngle, arcAngle, color, startA, finalA);
        }
        
    glPopMatrix();
}

void CenteredArc::drawLineArc(float x, float y, float radius, float thickness, float startAngle, float arcAngle, Color *color, float startAlpha, float finalAlpha) {
    glPushMatrix();
        glTranslatef(x, y, 0);
        drawLineArc(radius, thickness, startAngle, arcAngle, color, startAlpha, finalAlpha);        
    glPopMatrix();
}

void CenteredArc::drawPolygonArc(float x, float y, float radius, float thickness, float startAngle, float arcAngle, Color *color, float startAlpha, float finalAlpha) {
    glPushMatrix();
        glTranslatef(x, y, 0);
        drawPolygonArc(radius, thickness, startAngle, arcAngle, color, startAlpha, finalAlpha);        
    glPopMatrix();
}

void CenteredArc::drawToPick() {
    glPushMatrix();
        glTranslatef(x, y, 0);


        if (thickness < 5) {
            drawToPickAsLineStrips();
        } else {
            drawToPickAsPolygons();
        }
        
    glPopMatrix();
}

void CenteredArc::drawToPickAsLineStrips() {
    float num_segments = 360.0;

    float theta = arcAngle / num_segments; 
    float tangetial_factor = tanf(theta);//calculate the tangential factor 

    float radial_factor = cosf(theta);//calculate the radial factor 

    float xx = radius * cosf(startAngle);//we start at angle = 0 
    float yy = radius * sinf(startAngle); 

    glLineWidth(thickness);

    float prevXX = -1;
    float prevYY = -1;

    for(int ii = 0; ii < num_segments; ii++) 
    { 
        glColor3ub(pickR, pickG, pickB);
        if (prevXX != -1) {
            glBegin(GL_LINE_STRIP);
                glVertex2f(prevXX, prevYY);
                glVertex2f(xx, yy);
            glEnd();
        }

        //calculate the tangential vector 
        //remember, the radial vector is (x, y) 
        //to get the tangential vector we flip those coordinates and negate one of them 

        float tx = -yy; 
        float ty = xx; 

        prevXX = xx;
        prevYY = yy;

        //add the tangential vector 

        xx += tx * tangetial_factor; 
        yy += ty * tangetial_factor; 

        //correct using the radial factor 

        xx *= radial_factor; 
        yy *= radial_factor; 
    }

    glLineWidth(1);
}

void CenteredArc::drawToPickAsPolygons() {
    float num_segments = 360.0;

    float radiusOuter = radius + thickness / 2;
    float radiusInner = radius - thickness / 2;
    float theta = arcAngle / num_segments; 
    float tangetial_factor = tanf(theta);//calculate the tangential factor 

    float radial_factor = cosf(theta);//calculate the radial factor 
	
    float xxOuter = radiusOuter * cosf(startAngle);//we start at angle = 0 
    float yyOuter = radiusOuter * sinf(startAngle);
    float xxInner = radiusInner * cosf(startAngle);//we start at angle = 0 
    float yyInner = radiusInner * sinf(startAngle);  
    
    float prevXXOuter = -1;
    float prevYYOuter = -1;
    float prevXXInner = -1;
    float prevYYInner = -1;

    glPolygonMode(GL_FRONT, GL_FILL);
    for(int ii = 0; ii < num_segments; ii++) 
    { 
        glColor3ub(pickR, pickG, pickB);
        if (prevXXOuter != -1) {
            glBegin(GL_POLYGON);
                glVertex2f(prevXXOuter, prevYYOuter);
                glVertex2f(xxOuter,     yyOuter);
                glVertex2f(xxInner,     yyInner);
                glVertex2f(prevXXInner, prevYYInner);
            glEnd();
        }
		
        //calculate the tangential vector 
        //remember, the radial vector is (x, y) 
        //to get the tangential vector we flip those coordinates and negate one of them 

        float txOuter = -yyOuter; 
        float tyOuter =  xxOuter; 
        float txInner = -yyInner; 
        float tyInner =  xxInner; 
        
        //add the tangential vector 

        prevXXOuter = xxOuter;
        prevYYOuter = yyOuter;
        prevXXInner = xxInner;
        prevYYInner = yyInner;

        xxOuter += txOuter * tangetial_factor; 
        yyOuter += tyOuter * tangetial_factor; 
        xxInner += txInner * tangetial_factor; 
        yyInner += tyInner * tangetial_factor; 
        
        //correct using the radial factor 
        xxOuter *= radial_factor; 
        yyOuter *= radial_factor; 
        xxInner *= radial_factor; 
        yyInner *= radial_factor; 
    }
}

void CenteredArc::drawSelected() {
    if (selected) {
        float oldStart = startAlpha;
        float oldFinal = finalAlpha;
        startAlpha = 0.4;
        finalAlpha = 0.1;
        draw();
        startAlpha = oldStart;
        finalAlpha = oldFinal;
    }
}

void CenteredArc::drawLineArc(float radius, float thickness, float startAngle, float arcAngle, Color *color, float startAlpha, float finalAlpha) {
    float num_segments = 360.0;

    float theta = arcAngle / num_segments; 
    float tangetial_factor = tanf(theta);//calculate the tangential factor 

    float radial_factor = cosf(theta);//calculate the radial factor 

    float diff = startAlpha - finalAlpha;
    float alphaStep = diff / num_segments;

    float xx = radius * cosf(startAngle);//we start at angle = 0 
    float yy = radius * sinf(startAngle); 

    glLineWidth(thickness);

    float prevXX = -1;
    float prevYY = -1;

    for(int ii = 0; ii < num_segments; ii++) 
    { 
        glColor4f( color->r, color->g, color->b, startAlpha - ii * alphaStep );
        if (prevXX != -1) {
            glBegin(GL_LINE_STRIP);
                glVertex2f(prevXX, prevYY);
                glVertex2f(xx, yy);
            glEnd();
        }

        //calculate the tangential vector 
        //remember, the radial vector is (x, y) 
        //to get the tangential vector we flip those coordinates and negate one of them 

        float tx = -yy; 
        float ty = xx; 

        prevXX = xx;
        prevYY = yy;

        //add the tangential vector 

        xx += tx * tangetial_factor; 
        yy += ty * tangetial_factor; 

        //correct using the radial factor 

        xx *= radial_factor; 
        yy *= radial_factor; 
    }

    glLineWidth(1);
}

void CenteredArc::drawPolygonArc(float radius, float thickness, float startAngle, float arcAngle, Color *color, float startAlpha, float finalAlpha) {
    float num_segments = 360.0;

    float radiusOuter = radius + thickness / 2;
    float radiusInner = radius - thickness / 2;
    float theta = arcAngle / num_segments; 
    float tangetial_factor = tanf(theta);//calculate the tangential factor 

    float radial_factor = cosf(theta);//calculate the radial factor 
	
    float xxOuter = radiusOuter * cosf(startAngle);//we start at angle = 0 
    float yyOuter = radiusOuter * sinf(startAngle);
    float xxInner = radiusInner * cosf(startAngle);//we start at angle = 0 
    float yyInner = radiusInner * sinf(startAngle);  

    float diff = startAlpha - finalAlpha;
    float alphaStep = diff / num_segments;
    
    float prevXXOuter = -1;
    float prevYYOuter = -1;
    float prevXXInner = -1;
    float prevYYInner = -1;

    glPolygonMode(GL_FRONT, GL_FILL);
    for(int ii = 0; ii < num_segments; ii++) 
    { 
        glColor4f( color->r, color->g, color->b, startAlpha - ii * alphaStep);
        if (prevXXOuter != -1) {
            glBegin(GL_POLYGON);
                glVertex2f(prevXXOuter, prevYYOuter);
                glVertex2f(xxOuter,     yyOuter);
                glVertex2f(xxInner,     yyInner);
                glVertex2f(prevXXInner, prevYYInner);
            glEnd();
        }
		
        //calculate the tangential vector 
        //remember, the radial vector is (x, y) 
        //to get the tangential vector we flip those coordinates and negate one of them 

        float txOuter = -yyOuter; 
        float tyOuter =  xxOuter; 
        float txInner = -yyInner; 
        float tyInner =  xxInner; 
        
        //add the tangential vector 

        prevXXOuter = xxOuter;
        prevYYOuter = yyOuter;
        prevXXInner = xxInner;
        prevYYInner = yyInner;

        xxOuter += txOuter * tangetial_factor; 
        yyOuter += tyOuter * tangetial_factor; 
        xxInner += txInner * tangetial_factor; 
        yyInner += tyInner * tangetial_factor; 
        
        //correct using the radial factor 
        xxOuter *= radial_factor; 
        yyOuter *= radial_factor; 
        xxInner *= radial_factor; 
        yyInner *= radial_factor; 
    }
}