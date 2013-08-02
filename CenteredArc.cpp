#include "CenteredArc.h"
#include "Color.h"
#include <math.h>

CenteredArc::CenteredArc() : Point(0, 0) {
    radius = 10;
    thickness = 1;
    startAngle = 3.0 * M_PI / 2.0;
    arcAngle = M_PI;
    color = new Color(0, 0, 0, 0.25);
    quadratic = gluNewQuadric();
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


        glColor4f( color->r, color->g, color->b, color->a );
        if (thickness < 5) {
            drawAsLineStrips();
        } else {
            drawAsPolygons();
        }
        
    glPopMatrix();
}

void CenteredArc::drawAsLineStrips() {
    float num_segments = 360.0;

    float theta = arcAngle / num_segments; 
    float tangetial_factor = tanf(theta);//calculate the tangential factor 

    float radial_factor = cosf(theta);//calculate the radial factor 

    float xx = radius * cosf(startAngle);//we start at angle = 0 

    float yy = radius * sinf(startAngle); 

    glLineWidth(thickness);

    glBegin(GL_LINE_STRIP);
        for(int ii = 0; ii < num_segments; ii++) 
        { 
            glVertex2f(xx, yy);

            //calculate the tangential vector 
            //remember, the radial vector is (x, y) 
            //to get the tangential vector we flip those coordinates and negate one of them 

            float tx = -yy; 
            float ty = xx; 

            //add the tangential vector 

            xx += tx * tangetial_factor; 
            yy += ty * tangetial_factor; 

            //correct using the radial factor 

            xx *= radial_factor; 
            yy *= radial_factor; 
        }
    glEnd();

    glLineWidth(1);
}

void CenteredArc::drawAsPolygons() {
    float start = (startAngle + M_PI / 2) * 180 / M_PI;
    float arc = arcAngle * 180 / M_PI;
    gluPartialDisk(quadratic, radius - thickness / 2, radius + thickness / 2, 360, 100, start, arc);
}