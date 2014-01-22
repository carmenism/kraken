#include "CenteredArc.h"
#include "Color.h"
#include "Triangle.h"
#include <math.h>
#include <GL/glut.h>
#include <QtOpenGL>



CenteredArc::CenteredArc() : Point(0, 0), Pickable() {
    
}

CenteredArc::~CenteredArc() {
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



