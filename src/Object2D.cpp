#include <GL/glut.h>
#include "Object2D.h"
#include "Color.h"

Object2D::Object2D() {
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    borderColor = new Color(Color::gray);
}

Object2D::~Object2D() {
    delete borderColor;
}

void Object2D::drawBoundary() { 
    glDisable(GL_LINE_SMOOTH);
    glPolygonMode(GL_FRONT, GL_LINE);  
    glLineWidth(1.0);
    glColor4f(borderColor->r, borderColor->g, borderColor->b, borderColor->a);

    glBegin(GL_LINE_LOOP);
        glVertex2f( 0, 0 );
        glVertex2f( 0, height );
        glVertex2f( width, height );
        glVertex2f( width, 0 );
    glEnd();
    glEnable(GL_LINE_SMOOTH);
}